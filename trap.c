#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
  
  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    //cprintf("inside cpu %d \n",(int )(proc->tf->esp));
    if(proc->killed)
      exit();
    proc->tf = tf;
    syscall();
    if(proc->killed)
      exit();
    return;
  }

  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpu->id == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    if(proc && (tf->cs & 3) == 3 && proc->alarmval!=-1 && proc->sig_handler[1]!=(sighandler_t)-1)
    {
      proc->ticks++;
      if(proc->alarmval == proc->ticks)
      {
        proc->ticks = 0;
        *((int *)(tf->esp)-1)=(int)proc->sig_handler[1]; //place handler before eip as argument
        *((int *)(tf->esp)-2)=tf->eip; //place return address here
        proc->tf->esp-=8;
	proc->tf->eip=(uint)proc->sig_handler[2]; //0 before
      }
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpu->id, tf->cs, tf->eip);
    lapiceoi();
    break;
   
  //PAGEBREAK: 13
  default:
    if(proc == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpu->id, tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
 // check if signal handler assigned.
	else
	{
         if(tf->trapno==14 && proc->sig_handler[0]!=(sighandler_t)-1 )
	{
		//cprintf("inside the trap \n\n");
		//cprintf("address of esp is 0x%x \n",(int *)tf->esp);
		//cprintf("address of esp arg is 0x%x \n",((int *)(tf->esp)-1));
		//cprintf("address of esp ret is 0x%x \n",((int *)(tf->esp)-2));
		*((int *)(tf->esp)-1)=(int)proc->sig_handler[0]; //eip before
		*((int *)(tf->esp)-2)=tf->eip; //was not there before
		//cprintf("address of ip is 0x%x \n",tf->eip);
		//cprintf("value of esp arg is 0x%x \n",*((int *)(tf->esp)-1));
		//cprintf("value of esp ret is 0x%x \n",*((int *)(tf->esp)-2));
		proc->tf->esp-=8;
		proc->tf->eip=(uint)proc->sig_handler[2]; //0 before
		//cprintf("inside the trap 2 address of modified esp is 0x%x\n\n",tf->esp);
	}
       else
     	{
    	    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            proc->pid, proc->name, tf->trapno, tf->err, cpu->id, tf->eip,
            rcr2());
    		proc->killed = 1;
   	}
        }
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running 
  // until it gets to the regular system call return.)
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(proc && proc->state == RUNNING && tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();
}
