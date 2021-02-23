#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"

int g_var = 99;

int main(void)
{
	pid_t pid = fork();

	if(pid < 0)
	{
		printf("fork error");
		return -1;
	}
	if(pid > 0)
	{
		g_var = 0;
		printf("father pid == [%d] \t g_var == [%d]\n",getpid(),g_var);
		sleep(100);
	}
	if(pid == 0)
	{
		printf("child pid == [%d] \t g_var == [%d]\n",getpid(),g_var);
	}

	return 1;
}

