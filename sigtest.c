#include"types.h"
#include"user.h"
#include"wrapper.c"
#define SIGSEGV 0
int flag1;
uint t1,t2;
void handling(void)
{
	char i;
	char* j;
	flag1++;
	j=&i;
	/*for(i=0;i<80;i++)
	{
	 printf(1,"address is 0x%x, value is 0x%x \n",(j),(*j));
	 j++;
	}*/
	//exit();
	 t2=uptime();
	 j+=45;
	 *j+=3;
	 printf(1,"Seg Fault Occured, Handler called.\n\n");
	// printf(1,"address is 0x%x, value is 0x%x \n",(j+1),*(j+1));
		
} 
int 
main(void)
{
	signal(-1,(sighandler_t)wrapper);
	flag1=0;
	int value=10000;
	char i;
	char* j;
	i=signal(SIGSEGV,handling);
	if(i==1)
	//printf(1,"In user proc- setting up handler worked worked \n\n");
	j=&i;
	//printf(1,"address in main is 0x%x \n\n",main);
	j+=value;	
	//t1=uptime();
	*j=0;
	printf(1,"program successfully back in the main routine, will terminate now as has nothing to do \n\n");
	//printf(1,"time is %d uS\n",((t2-t1)*100*1000)/100000);
	exit();
}
