#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"

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
		printf("father: pid == [%d],fpid == [%d]\n",getpid(),getppid());	
	}
	if(pid == 0)
	{
		printf("child: pid == [%d],fpid == [%d]\n",getpid(),getppid()); 
		execlp("ls","ls","-l",NULL);
		// execlp("./1-fork","1-fork",NULL); 
		// execl("./test","test","hello",NULL);
		// execlp("./test","test","hello",NULL);
		perror("execl error"); 
	}
	sleep(1);

	return 1;
}
