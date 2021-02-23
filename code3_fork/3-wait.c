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
	else if(pid > 0)
	{
		printf("father pid == [%d]\n",getpid());
		int status;
		pid_t wpid = wait(&status);
		printf("wpid == [%d]\n",wpid);
	    if(wpid > 0)	
		{
			if(WIFEXITED(status))
			{
				printf("child exit noemal,status == [%d]\n",WEXITSTATUS(status));
			}
			else if(WIFSIGNALED(status))
			{
				printf("child exit signel,signo == [%d]\n",WTERMSIG(status));
			}
		}
	}
	if(pid == 0)
	{
		printf("child: pid == [%d],fpid == [%d]\n",getpid(),getppid());  
		sleep(200);
		
		return 9;
	}
	sleep(1);

	return 1;
}
