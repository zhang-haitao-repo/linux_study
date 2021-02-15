#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

int main()
{
    // 创建fifo
    int ret = access("./myfifo", F_OK);
    if(ret != 0)
    {
        int ret = mkfifo("./myfifo", 0777);
        if(ret < 0)
        {
            perror("mkfifo error");
            return -1;
        }
    }

    // 打开文件
    int fd = open("./myfifo", O_RDWR);
    if(fd < 0)
    {
        perror("open myfifo error");
    }

    while (1)
    {
        char str[64];

        fgets(str, 64, stdin);
        // 写fifo文件
        write(fd, str, strlen(str));
        // sleep(1);
    }

    // 关闭fifo文件
    close(fd);

    getchar();

    return EXIT_SUCCESS;
}

