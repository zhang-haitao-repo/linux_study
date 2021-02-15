#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

int main()
{
    // 打开文件
    int fd = open("./myfifo", O_RDWR);
    if(fd < 0)
    {
        perror("open myfifo error");
    }

    // 读fifo文件
    char buf[64];
    memset(buf, 0x00, sizeof(buf));
    int n = 0;
    while(1)
    {
        n = read(fd, buf, sizeof(buf));
        printf("n:%d,s:%s\n",n, buf); 
    }

    return EXIT_SUCCESS;
}

