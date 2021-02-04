#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

int main()
{
	pid_t pid; 
	for(int i = 0; i < 3; ++i)
	{
		pid = fork();
		if(pid == 0)
		{
			break;
		}
	}
	if(pid < 0)
	{
		printf("fork error");
		return -1;
	}
	else if(pid > 0)
	{
		printf("father pid == [%d]\n",getpid());
		int status;
		//pid_t wpid = waitpid(pid, &status, 0);
		while(1)
		{
			// -1表示等待任意子进程，WNOHANG表示不阻塞
			pid_t wpid = waitpid(-1, &status, WNOHANG); 
			// printf("wpid == [%d]\n",wpid);
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
			else if (wpid == 0)
			{
				// printf("child is living, wpid==[%d]\n",wpid);
			}
			else if (wpid == -1)
			{
				printf("no child is living,wpid == [%d]\n",wpid);
				break;
			}
		}
		
	}
	if(pid == 0)
	{
		printf("child: pid == [%d],fpid == [%d]\n",getpid(),getppid());  
		sleep(1);
		return 9;
	}
	sleep(1);

	return 1;
}
