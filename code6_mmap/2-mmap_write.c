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

	memset(addr, 0x00, len);
	memcpy(addr, "[hello 123456]", strlen("[hello 123456]"));	

	return EXIT_SUCCESS;
}

