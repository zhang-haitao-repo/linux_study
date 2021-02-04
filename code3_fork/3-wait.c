#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

int main()
{
	pid_t pid = fork();
    
    if(pid < 0)
	{
		printf("fork error");
		return -1;
	}
	if(pid > 0)
	{
		pid_t wpid = wait(NULL);
		printf("father: pid == [%d],fpid == [%d]\n",getpid(),getppid());
	}
	if(pid == 0)
	{
		printf("child: pid == [%d],fpid == [%d]\n",getpid(),getppid());  
		sleep(1);
	}
	sleep(1);

	return 1;
}
