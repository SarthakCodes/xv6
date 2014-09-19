
#include"types.h"
#include"user.h"

int main()
{
	int n,i;
 	struct procentry
        {
                int id;
                char state[10];
                char name[16];
        } q[64],*p;
	p=&q[0];

	n=getproc((int)p);
        printf(1,"pid|  state     |name\n\n");
	for (i=0;i<n;i++)
	{
		printf(1,"%d |  %s   |%s\n",p[i].id,p[i].state,p[i].name); 
	}
	exit();
}
