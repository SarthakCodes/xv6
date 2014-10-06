#include"types.h"
#include"user.h"
#include"wrapper.c"
#define SIGALARM 1
//int flag1;
//uint t1,t2;
void handling(void)
{
	 printf(1,"Alarm Triggered.\n\n");		
} 
int 
main(void)
{
	signal(-1,(sighandler_t)wrapper);
        int i;
        alarm(50);
	i=signal(SIGALARM,handling);
        for(i=0;i<50000000;i++);
	exit();
}
