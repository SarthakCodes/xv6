#include"types.h"
#include"user.h"
void wrapper(sighandler_t f)
{
	
 __asm__ ("push %eax\n\t"
          "push %ecx\n\t"
          "push %edx\n\t");
	//printf(1,"I am here in wrapper \n");
	   f();
	//printf(1,"I am here again \n");
*((int*)&f) = *(((int*)&f)-1);    
*(((int*)&f)-1) = *(((int*)&f)-2);  
asm("pop %edx"); 
asm("pop %ecx"); 
asm("pop %eax");
asm("addl $4,%ebp");
}
