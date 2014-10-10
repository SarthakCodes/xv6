#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#define IO_TIMER1       0x040           // 8253 Timer #1

// Frequency of all three count-down timers;
// (TIMER_FREQ/freq) is the appropriate count
// to generate a frequency of freq Hz.

#define TIMER_FREQ      1193182
#define TIMER_DIV(x)    ((TIMER_FREQ+(x)/2)/(x))

#define TIMER_MODE      (IO_TIMER1 + 3) // timer mode port
#define TIMER_SEL0      0x00    // select counter 0
#define TIMER_RATEGEN   0x04    // mode 2, rate generator
#define TIMER_16BIT     0x30    // r/w counter 16 bits, LSB first

#define LATCH_COUNTER0 0x00
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
//return uptime in microseconds
int
sys_microuptime(void)
{
int microticks=0;
if(cpu->id == 0)
{ 
//if(!ismp)
//{ 
acquire(&mtickslock);
uint LSB,MSB;
//for(i=0;i<15;i++)
//{
outb(0x43,0x00);
LSB=(uint)inb(0x40);
MSB=(uint)inb(0x40);
microticks=((MSB<<8)|LSB);//+(ticks*10000);//mticks;//+(ticks*10000);
//cprintf("0x%x , 0x%x ,%d \n",MSB,LSB,microticks);
//}
release(&mtickslock); 
} 
return microticks;
}
int sys_getproc(void)
{
	int sadd;
	
  	if(argint(0, &sadd) < 0)
    		return -1;
	return(getproc(sadd));
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}


/*int sys_mygetproc(void)
{
int sadd,n,i,flag=0;
struct procentry{
int id;
char state[10];
char name[16];
}*pe;
char check[16];
safestrcpy(check, "badproc", sizeof("badproc"));
 if(argint(0, &sadd) < 0)
  return -1;
 n=getproc(sadd);
//cprintf("%d \n\n",n);
 
pe=(struct procentry *)sadd;
for(i=0;i<n;i++)
{
 if (!strcmp((pe+i)->name,check))
  {
   flag++;
   (pe+i)->id=(pe+ (n-flag))->id;
   safestrcpy((pe+i)->name, (pe+ (n-flag))->name, sizeof((pe+ (n-flag))->name));
   safestrcpy((pe+i)->state, (pe+ (n-flag))->state, sizeof((pe+ (n-flag))->state));
  }
}
if(flag>0)
return (n-flag);
else
return n;
}*/

int
sys_sysreplace(void)
{
 int num;
 int func_addr;
 int old_func_addr;
 if(argint(0,&num) < 0)
 return -1; 
 if(argint(1,&func_addr) < 0)
 return -1; 
 if(argint(2,&old_func_addr) < 0)
 return -1;
 return (sysreplace(num,func_addr,old_func_addr));
}

int
sys_findkalloc(void)
{
  return((int)kalloc);
}

int
sys_signal(void)
{
	int signum;
	int handler;
	if(argint(0, &signum) < 0)
        return -1;
	if(signum<-1 || signum>1) //signum<0 before
	return -1;
	if(argint(1, &handler) < 0)
	return -1;
	return(signal(signum,(sighandler_t)handler));	//signal implementation in proc.c
}

int
sys_alarm(void)
{
 int alarmvalue;
 if(argint(0, &alarmvalue) < 0)
  return -1;
  return (alarm(alarmvalue));
}
