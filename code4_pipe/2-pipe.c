#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"

int main()
{
    //创建管道
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0)
    {
        perror("pipe error");
        return -1;
    }
    
    //创建子进程
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork error");
        return -1;
    }
    else if(pid > 0)
    {
        //关闭读端
        close(fd[0]);
        //将标准输出重定向到写端
        dup2(fd[1], STDOUT_FILENO);

        execlp("ps","ps","aux",NULL);
    }
    else
    {
        //关闭写端
        close(fd[1]);
        //将标准输入重定向到读端
        dup2(fd[0], STDIN_FILENO);

        execlp("grep","grep","bash",NULL);

        char buf[1025];
        memset(buf,0x00,sizeof(buf));
        int n = read(fd[0],buf,sizeof(buf));

        printf("read over n == [%d]\n %s\n", n, buf);
    }

    return EXIT_SUCCESS;
}

