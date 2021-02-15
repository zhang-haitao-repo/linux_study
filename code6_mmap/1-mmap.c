// 使用mmap进行父子进程间的通信。
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/mman.h"

int main()
{
    int fd = open("./test.log", O_RDWR);
    // 使用mmap函数建立共享映射区
    int len = lseek(fd, 0, SEEK_END);

    void *addr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        perror("mmap error");
    }

    // 创建子进程
	pid_t pid = fork();
    
    if(pid < 0)
	{
		printf("fork error");
		return -1;
	}
	if(pid > 0)
	{
		// printf("father: pid == [%d],fpid == [%d]\n",getpid(),getppid());
        memcpy(addr, "[hello zhang HAI TAO]", strlen("[hello zhang HAI TAO]"));	
	}
	if(pid == 0)
	{
		// printf("child: pid == [%d],fpid == [%d]\n",getpid(),getppid());
        sleep(1);
        char *p = (char*)addr;
        printf("%s", p);
	}
	// sleep(1);

	return 1;
}

