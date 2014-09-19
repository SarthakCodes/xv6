#include"types.h"
#include"user.h"
#include"fcntl.h"
#define stdout 1
int main()
{
int pid;
pid=fork();
if(pid==0)
{
   int fd;
   while(1)
   {
   fd = open("badproc.out", O_CREATE|O_RDWR);
   if(fd < 0){
    printf(stdout, "error: creating badproc.out\n");
    exit();
    }
  printf(fd, "%d ticks\n",uptime());
  sleep(1000);
 close(fd);
}
}
else
{
exit();
}
}
