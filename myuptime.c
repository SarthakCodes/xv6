
//Implentes a root kit attack on the kernel and replaces the system call uptime() with a user program that runs from kernel space.
// This is very COOL! 
#include"types.h"
#include"user.h"
int (*f)(void);
int myuptime(void);
int main(void)
{
int old_func_add;
//printf(1,"the add of global is 0x%x \n",*(&old_func_add));
printf(1,"the value of uptime is %d \n",uptime());
sysreplace(14,(int)myuptime,(int)&old_func_add);
f=(void *)old_func_add;
//printf(1,"I am back and add is 0x%x \n\n",(void *)(old_func_add));
printf(1,"the new value of uptime is %d \n",uptime());
exit();
}
int myuptime(void)
{
  return (50000+f());
}

