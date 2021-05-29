# 1 编译流程及库的制作

## 1.1 编译流程

### 1.1.1 gcc的工作流程:

![img](file:///C:\Users\ADMINI~1\AppData\Local\Temp\ksohtml4980\wps46.png)

1 预处理: cpp预处理器, 去掉注释, 展开头文件, 宏替换
	gcc -E test.c -o test.i
2 编译: gcc, 将源代码文件编译成汇编语言代码
	gcc -S test.i -o test.s
3 汇编: as, 将汇编语言代码编译成了二进制文件(目标代码)
	gcc -c test.s -o test.o
4 链接: ld, 链接test.c代码中调用的库函数
	gcc -o test test.o

一步生成最终的可执行程序: 
gcc test.c -o test

![img](file:///C:\Users\ADMINI~1\AppData\Local\Temp\ksohtml4980\wps17.png)

### 1.1.2 gcc常用参数

-v  查看gcc版本号, --version也可以

-E  生成预处理文件

-S  生成汇编文件

v-c  只编译, 生成.o文件, 通常称为目标文件

-I   指定头文件所在的路径

-L   指定库文件所在的路径

-l    指定库的名字

-o   指定生成的目标文件的名字

-g   包含调试信息, 使用gdb调试需要添加-g参数

-On n=0∼3 编译优化,n越大优化得越多

-Wall 提示更多警告信息

-D  编译时定义宏


## 1.2 静/动态库

### 1.2.1 静态库

库是二进制文件, 是源代码文件的另一种表现形式, 是加了密的源代码; 

- 提高代码的可重用性, 而且还可以提高程序的健壮性;
- 可以减少开发者的代码开发量, 缩短开发周期.
- 头文件---包含了库函数的声明
- 库文件---包含了库函数的代码实现

步骤2：使用打包工具ar将准备好的.o文件打包为.a文件，在使用ar工具是时候需要添加参数rcs

- r更新、c创建、s建立索引
- 命令：ar rcs 静态库名 .o文件

![image-20210121112043562](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210121112043562.png)

**静态库的制作:**

- 1 将.c文件编译成.o文件
  		gcc -c fun1.c fun2.c
- 2 使用ar命令将.o文件打包成.a文件
  		ar rcs libtest1.a fun1.o fun2.o

**静态库的使用:**

- 1 main.c与head.h和libtest1.a在同一级目录的情况
  		gcc -o main1 main.c -I./ -L./ -ltest1	
- 2 main.c与head.h和libtest1.a在不同一级目录的情况
  		gcc -o main1 main.c -I./include -L./lib -ltest1

**静态库的优缺点**

- 优点：
  - 函数库最终被打包到应用程序中，实现是函数本地化，寻址方便、速度快。
  - （库函数调用效率==自定义函数使用效率）
  - 程序在运行时与函数库再无瓜葛，移植方便。
- 缺点：
  - 消耗系统资源较大, 每个进程使用静态库都要复制一份, 无端浪费内存。

![image-20210121141151499](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210121141151499.png)

### 1.2.2 动态库

- 共享库在程序编译时并不会被连接到目标代码中, 而是在程序运行是才被载入. 

- 不同的应用程序如果调用相同的库, 那么在内存里只需要有一份该共享库的拷贝, 规避了空间浪费问题.

- 动态库在程序运行时才被载入, 也解决了静态库对程序的更新、部署和发布会带来麻烦。

- 用户只需要更新动态库即可, 增量更新. 为什么需要动态库, 其实也是静态库的特点导致. 

  

  ![image-20210121153622972](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210121153622972.png)

**动态库的制作**

- 生成目标文件.o, 此时要加编译选项：-fPIC（fpic）
  - gcc -fpic -c fun1.c fun2.c
  - 参数：-**fpic**创建与地址无关的编译程序, 目的就是为了能够在多个应用程序间共享.
- 生成共享库, 此时要加链接器选项: -shared（指定生成动态链接库）
  - gcc -shared fun1.o fun2.o -o libtest2.so

**动态库的使用**

- -L：指定要连接的库的所在目录
- -l：指定链接时需要的动态库, 去掉前缀和后缀
- -I: 指定main.c文件用到的头文件head.h所在的路径
- gcc main.c -I./ -L./ -ltest2 -o main2

> 对于elf格式的可执行程序，是由ld-linux.so*来完成的, 它先后搜索elf文件的 DT_RPATH段 — 环境变量LD_LIBRARY_PATH — /etc/ld.so.cache文件列表 — /lib/, /usr/lib目录找到库文件后将其载入内存。

**如何让系统找到共享库**

- 临时设置LD_LIBRARY_PATH:
  - export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:库路径 
- **永久设置, 把export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:库路径, 设置到∼/.bashrc文件中, 然后在执行下列三种办法之一:**
  - **执行. ~/.bashrc使配置文件生效(第一个.后面有一个空格)**
  - **执行source ~/.bashrc配置文件生效**
  - **退出当前终端, 然后再次登陆也可以使配置文件生效**
- 永久设置,把export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:库路径，设置到/etc/profile文件中(不建议)



### 1.2.3 优缺点

 **静态库的优点:**
 	1 执行速度快, 是因为静态库已经编译到可执行文件内部了
 	2 移植方便, 不依赖域其他的库文件
 **缺点:** 
 	1 耗费内存, 是由于每一个静态库的可执行程序都会加载一次
 	2 部署更新麻烦, 因为静态库修改以后所有的调用到这个静态库的可执行文
件都需要重新编译

 **动态库的优点:**
 	1 节省内存
 	2 部署升级更新方便, 只需替换动态库即可, 然后再重启服务.（**大型程序无需重新编译**）
 **缺点:** 
 	1 加载速度比静态库慢
 	2 移植性差, 需要把所有用到的动态库都移植.

由于由静态库生成的可执行文件是把静态库加载到了其内部, 所以静态库生成的可执行文件一般会比动态库大.



# 2 makefile和gdb

## 2.1 makefile

**makefile：**管理项目工程文件。

**makefile的编写:**
**规则:**

目标: 依赖
(tab)命令

第一个版本:

```makefile
main: main.c fun1.c fun2.c sum.c
	gcc -o main main.c fun1.c fun2.c sum.c
```

第二个版本:

```makefile
main: main.o func1.o func2.o sum.o
	gcc -o main main.o func1.o func2.o sum.o
main.o: main.c
	gcc -c main.c -I./
func1.o: func1.c
	gcc -c func1.c
func2.o: func2.c
	gcc -c func2.c
sum.o: sum.c
	gcc -c sum.c
```

   ![image-20210121165600699](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210121165600699.png)

第三个版本:
	变量: 
		自定义变量: var = hello, $(var)
		自带变量: CC CPPFLAGS CFLAGS LDFLAGS
		自动变量: $@目标  $<第一个条件  $^所有条件
	模式规则:
		%.o:%.c------> 前后的%必须是相同 

```makefile
target = main
object = main.o func1.o func2.o sum.o
CC = gcc
CPPFLAGS=-I./
$(target): $(object)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) -c $< $(CPPFLAGS)
```



第四个版本:		
	1.wildcard – 查找指定目录下的指定类型的文件
	src=$(wildcard *.c)  //找到当前目录下所有后缀为.c的文件,赋值给src
	2.patsubst – 匹配替换
	obj=$(patsubst %.c,%.o, $(src)) //把src变量里所有后缀为.c的文件替换成.o

```makefile
src = $(wildcard ./*c)
object = $(patsubst %.c,%.o,$(src))
CC = gcc
CPPFLAGS=-I./
$(target): $(object)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) -c $< $(CPPFLAGS)
```

第五个版本:
	增加清理功能.
	终极目标: makefile文件中第一次出现的目标叫做终极目标

```makefile
src = $(wildcard ./*c)
object = $(patsubst %.c,%.o,$(src))
CC = gcc
CPPFLAGS=-I./
$(target): $(object)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) -c $< $(CPPFLAGS)
.PHONY:clean
clean:
	rm -f $(object) $(target)
		
```

– make -f : -f执行一个makefile文件名称, 使用make执行指定的makefile: make -f mainmak

# 3 IO函数

## 3.1 c语言操作文件相关问题:

使用fopen函数打开一个文件, 返回一个FILE* fp, 这个指针指向的结构体有三个重要的成员.

Ø 文件描述符: 通过文件描述可以找到文件的inode, 通过inode可以找到对应的数据块

Ø 文件指针: 读和写共享一个文件指针, 读或者写都会引起文件指针的变化

Ø 文件缓冲区: 读或者写会先通过文件缓冲区, 主要目的是为了减少对磁盘的读写次数, 提高读写磁盘的效率.

![image-20210123190550271](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210123190550271.png)



![image-20210123195754175](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210123195754175.png)

虚拟地址空间:
	进程的虚拟地址空间分为用户区和内核区, 其中内核区是受保护的, 用户是不能够对其进行读写操作的;
	内核区中很重要的一个就是进程管理, 进程管理中有一个区域就是PCB(本质是一个结构体);
	PCB中有文件描述符表, 文件描述符表中存放着打开的文件描述符, 涉及到文件的IO操作都会用到这个文件描述符.



> 作业: 
>
> 将add.c sub.c mul.c divd.c编写makefile文件生成库文件;-----makefile的名字为mathmak, 并编写main.c主程序调用库文件的makefile, 名字为mainmak.



## 3.2 c++中文件操作:

### 3.2.1 读文件

```c++
#include "fstream"

std::fstream ifs;

void read(){
	ifs.open("./tujiao.txt", ios::in);
    if(!ifs.is_open())
    {
        cout << "file not open" << endl;
    }
    while(getline(ifs, buf))
    {
        vector<float> tokens = Split(buf,',');
        motor_data1.push_back(tokens);
        i++;
    }
    ifs.close();
}
```



# 4 Linux下的进程

进程间不共享全局变量，共享文件。

全局变量：读时共享，写时复制。

文件：父子进程可以共享文件描述符。

## 4.1 并发和并行

- 并发，在一个时间段内, 是在同一个cpu上, 同时运行多个程序。
  - 如：若将CPU的1S的时间分成1000个时间片，每个进程执行完一个时间片必须无条件让出CPU的使用权，这样1S中就可以执行1000个进程。

- 并行性指两个或两个以上的程序在同一时刻发生(需要有多颗，与cpu核心数有关)。

## 4.2 进程状态(面试考)

- 进程基本的状态有5种。
- 分别为**初始态，就绪态，运行态，挂起态与终止态**。其中初始态为进程准备阶段，常与就绪态结合来看。

![image-20210204165631079](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210204165631079.png)

## 4.2 fork函数

```c
pid_t pid = fork();   // 创建子进程
if(pid < 0)
{
    printf("fork error");
}
if(pid > 0)
{
    printf("father pid %d",getpid());
}
if(pid == 0)
{
    printf("child pid");
}
```

**注意：对于全局变量读时共享，写时赋值**

## 4.2 exec函数族

> 有的时候需要在一个进程里面执行其他的命令或者是用户自定义的应用程序，此时就用到了exec函数族当中的函数。
>
> 使用方法一般都是在父进程里面调用fork创建处子进程，然后在子进程里面调用exec函数。

- 函数原型: int execl(const char *path, const char *arg, ... /* (char  *) NULL */);

- 参数介绍：
  - path: 要执行的程序的绝对路径
  - 变参arg: 要执行的程序的需要的参数
  - arg:占位，通常写应用程序的名字
  - arg后面的: 命令的参数
  - 参数写完之后: NULL
- 返回值：若是成功，则不返回，不会再执行exec函数后面的代码；若是失败，会执行execl后面的代码，可以用perror打印错误原因。
- execl函数一般执行自己写的程序。

> 总结：
>
> exec函数是用一个新程序替换了当前进程的代码段、数据段、堆和栈；原有的进程空间没有发生变化，并没有创建新的进程，进程PID没有发生变化。

函数练习

```c
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

```

## 4.3 进程回收函数（wait/waitpid）

僵尸进程：进程终止，父进程尚未回收，子进程残留资源（PCB）被存放于内核中，变成僵尸进程。

孤儿进程：父进程先于子进程结束，则子进程成为孤儿进程，子进程的父进程成为init进程，称为init进程领养孤儿进程。

- 函数原型：
  - **pid_t wait(int *status);**
- 函数作用：
  - 阻塞并等待子进程退出 
  - 回收子进程残留资源 
  - 获取子进程结束状态(退出原因)。
- 返回值：
  - 成功：清理掉的子进程ID；
  - 失败：-1 (没有子进程)
- status参数：子进程的退出状态 -- 传出参数
  - WIFEXITED(status)：为非0    → 进程正常结束
  - WEXITSTATUS(status)：获取进程退出状态 
  - WIFSIGNALED(status)：为非0 → 进程异常终止
  - WTERMSIG(status)：取得进程终止的信号编号。

```c
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
```

- 函数原型：
  - **pid_t waitpid(pid_t pid, int *status, in options);**
- 函数作用
  - 同wait函数
- 函数参数：
- pid：
  - pid = -1 等待任一子进程。与wait等效。
  - pid > 0 等待其进程ID与pid相等的子进程。
  - pid = 0 等待进程组ID与目前进程相同的任何子进程，也就是说任何和调用waitpid()函数的进程在同一个进程组的进程。
  - pid < -1 等待其组ID等于pid的绝对值的任一子进程。(适用于子进程在其他组的情况)
- status: 子进程的退出状态，用法同wait函数。
- options：设置为WNOHANG，函数非阻塞，设置为0，函数阻塞。
- 函数返回值
  - \>0：返回回收掉的子进程ID；
  - -1：无子进程
  - =0：参3为WNOHANG，且子进程正在运行。

```c
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
```

**注意：调用一次wait或waitpid函数只能回收一个子进程。**

# 5 进程间通信(管道及共享内存)

1. 熟练使用pipe进行父子进程间通信

2. 熟练使用pipe进行兄弟进程间通信

3. 熟练使用fifo进行无血缘关系的进程间通信
4. 使用mmap进行有血缘关系的进程间通信
5. 使用mmap进行无血缘关系的进程间通信

**基本概念（IPC）**:InterProcess Communication

- Linux环境下，进程地址空间相互独立，每个进程各自有不同的用户地址空间。任何一个进程的全局变量在另一个进程中都看不到。

- 两个进程要想完成数据交换，必须通过内核。

- 一个进程将数据写入内核，然后另一个进程从内核读走数据。

## 5.1 进程间通信的方式

文件，管道，信号，共享内存，消息队列，套接字等。

常用的有：

1. 管道（使用最简单）
2. 信号（开销最小）
3. 共享映射区（无血缘关系）
4. 本地套接字（最稳定）

## 5.2 无名管道

管道是一种最基本的IPC机制，应用于有血缘关系的进程间通信，完成数据传递。调用pipe函数创建一个管道。

管道的特质：

- 管道的本质是一块内核缓冲区。
- 有两个文件描述符引用，一个表示读端，一个表示写端。
- 规定数据从管道的写端流入管道，从读端流出。
- 当两个进程都终结时，管道也自动消失。
- 管道的读端和写端都默认是阻塞的。

管道的原理：

- 管道的是指是内核缓冲区，内部使用环形队列实现。
- 默认缓冲区大小为4K，可以使用ulimit -a命令获取大小。
- 数据流向单向（若想双向需要两个管道）

### 5.2.1 管道的读写行为

- 读操作
  - 有数据
    - read正常读，返回读出的字节数
  - 无数据
    - 写端全部关闭
    - read接触阻塞，返回0，相当于读文件读到了尾部。
- 写操作
  - 读端全部关闭
    - 管道破裂，进程终止，内核给当前进程发SIGPIPE信号
  - 读端没全部关闭
    - 缓冲区写满了，发生write阻塞。
    - 缓冲区没满，继续write。

### 5.2.2 如何设置管道非阻塞

默认情况下，管道的读写两端都是阻塞的，若要设为非阻塞：

1. int flags = fcntl(fd[0],F_GETFL);
2. flags |= O_NONBLOCK;
3. fcntl(fd[0],F_SETFL,flags);

若读端设置为非阻塞：

1. 写端没关闭，管道中没有数据可读，返回-1。
2. 写端没关闭，管道中有数据，返回实际读到的字节数。
3. 写端已关闭，管道中有数据，返回实际读到的字节数。
4. 写端已关闭，管道中没数据，则返回0。

**查看管道缓冲区大小**

命令：ulimit -a

函数：long fpathconf(int fd, int name);   **long fpathconf(fd[0], _PC_PIPE_BUF); **

## 5.3 命名管道

FIFO常被称为命名管道，以区分pipe。管道（pipe）只能用于有血缘关系的进程间通信。但是通过FIFO，不相关的进程也能交换数据。

FIFO是Linux基础文件类型中的一种（文件类型为p，可通过ls -l查看文件类型）。但FIFO文件在磁盘上没有数据块，文件大小为0，仅仅用来标识内核中一条通道。进程可以打开这个文件进行read/write，实际上是在读写内核缓冲区，这样就实现了进程间通信。

### 5.3.1 FIFO完成两个进程间通信的思路

进程A：

- 创建fifo文件，mkfifo命令或者mkfifo函数。
- 打开FIFO文件，open fifo文件获得文件描述符。
- write(fd, "xxxx", ...);写fifo文件。
- close(fd); 关闭FIFO文件。

进程B：

1. 打开fifo文件，获得文件描述符。
2. 读fifo文件，read(fd, buf, sizeof(buf));
3. 关闭fifo文件，close(fd);

### 5.3.2 代码段

write代码:

```c
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
```

read代码：

```c
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
```

## 5.4 内存映射区(mmap)

### 5.4.1 相关函数

```c
void *mmap(void *addr, size_t length, int port, int flags, int fd, off_t offset);
```
**作用：**建立存储映射区。

**参数：**

- addr：一般传NULL，表示让内核去指定一个内存起始地址。
- length：文件大小；
  - lseek或者stat函数得到。
- prot：
  - PROT_READ  PROT_WRITE  PROT_READ|PROT_WRITE  
- flags：
  - MAP_SHARED：对映射区的修改会反映到文件中（可以对文件进行修改）。
  - MAP_PRIVATE：对映射区的修改不会对文件产生影响。
- fd：打开文件描述符
  - fd = open();
- offset：从文件的哪个位置开始映射，一般传0。

**返回值：**映射区首地址。

```c
int munmap(void *addr, size_t length);
```
**作用：**释放由mmap创建的内存映射区。
**返回值：**

- 成功：返回0.
- 失败：返回1.

**参数：**

- addr：调用mmap函数成功返回的映射区首地址。

- length：映射区大小。（mmap函数第二个参数）。

### 5.4.2 代码演示

```c
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
```

```c
// 使用mmap进行不同进程间的通信。（写）
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
```

```c
// 使用mmap进行不同进程间的通信。（读）
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

    char *p = (char*)addr;
    printf("%s", p);	

	return EXIT_SUCCESS;
}
```

### 5.4.3 mmap函数建立匿名映射

```c
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
    void *addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED)
    {
        perror("mmap error");
    }

    // 创建子进程cd 
	pid_t pid = fork();
    
    if(pid < 0)
	{
		printf("fork error");
		return -1;
	}
	if(pid > 0)
	{
        memcpy(addr, "[hello zhang HAI TAO]\n", strlen("[hello zhang HAI TAO]\n"));	
		wait(NULL);
	}
	if(pid == 0)
	{
        sleep(1);
        char *p = (char*)addr;
        printf("%s", p);
	}

	return 1;
}
```

# 6 进程间通信(信号)

## 6.1 学习目标

- 了解信号中的基本概念
- 熟练使用信号相关的函数
- 参考文档使用信号集操作相关函数
- 熟练使用信号捕捉函数signal
- 熟练使用信号捕捉函数sigaction
- 熟练掌握使用信号完成子进程的回收

## 6.2 信号的机制

### 6.2.1 基本概念

**信号的概念：**

信号是信息的载体，Linux/UNIX 环境下，古老、经典的通信方式， 现下依然是主要的通信手段。

**信号的机制：**

进程A给进程B发送信号，进程B收到信号之前执行自己的代码，收到信号后，不管执行到程序的什么位置，都要暂停运行，去处理信号，处理完毕后再继续执行。与硬件中断类似——异步模式。但信号是软件层面上实现的中断，早期常被称为“软中断”。

**每个进程收到的所有信号，都是由内核负责发送的。(用户->内核->用户)**

### 6.2.2 信号的状态

信号有三种状态：产生、未决和递达。

- **信号的产生**
  - 按键产生，如：Ctrl+c、Ctrl+z、Ctrl+\
  - 系统调用产生，如：kill、raise、abort
  - 软件条件产生，如：定时器alarm
  - 硬件异常产生，如：非法访问内存(段错误)、除0(浮点数例外)、内存对齐出错(总线错误)
  - 命令产生，如：kill命令

- 未决：产生和递达之间的状态。主要由于阻塞(屏蔽)导致该状态。 
- 递达：递送并且到达进程。

**信号的处理方式：**

1. 执行默认动作。
2. 系统调用产生，如：kill、raise、abort。
3. 软件条件产生，如：定时器alarm。
4. 硬件异常产生，如：非法访问内存(段错误)、除0（浮点数例外）、内存对齐出错（总线错误）。
5. 命令产生，如：kill命令。

​    Linux内核的进程控制块PCB是一个结构体，task_struct，除了包含进程id，状态，工作目录，用户id，组id，文件描述符，还包含了信号相关的信息，主要指阻塞信号集和未决信号集。

- **阻塞信号集：**将某些信号加入集合，对他们设置屏蔽，当屏蔽x信号后，在收到该信号，该信号的处理将推后（解除屏蔽后）。
- **未决信号集：**1.信号产生，未决信号集中描述该信号的位立刻翻转为1，表信号处于未决状态。当信号被处理对应位翻转回0.这一时刻往往非常短暂。2.信号产生后由于某些原因（主要是阻塞）不能抵达。

**信号的编号：**

查看所有信号的指令：kill -l

**信号四要素：**

1.编号 2.名称 3.事件 4.默认处理动作 （通过man 7 singal查看）。

**相关函数**

```c
int kill(pid_t pid, int sig);
```

成功返回0，失败返回-1.

pid>0：发送信号给指定进程。

pid = 0：发送信号给与调用kill函数同一进程组的所有进程。

pid = -1：发送信号给进程有权限发送的系统中所有进程。

**alarm函数：**

作用：设置定时器，在指定second后，内核会给当前进程发送14）SIGALRM信号。

```c
unsigned int alarm(unsigned int seconds);
```

返回0或剩余的秒数，无失败。

**setitimer函数：**

作用：设置定时器。可代替alarm函数。精度为微秒，可实现周期性定时。

```c
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
```

参数：

- which：指定定时方式。1.自然定时：ITIMER_REAL->14)SIGVTAALRM。2.虚拟空间定时：（用户空间）3.运行时计时：（用户+内核）。

## 6.3 信号集操作

内核通过读取未决信号集来判断信号是否被处理。信号屏蔽字mask可以影响未决信号集。而我们可以在应用程序中自定义set来改变mask。已达到屏蔽指定信号的目的。

### 6.3.1 信号集设定

```c
sigset_t set;
int sigemptyset(sigset_t *set);	将某个信号集清0
int sigfillset(sigset_t *set);	将某个信号集置1
int sigaddset(sigset_t *set, int signum);	将某个信号加入信号集
int sigdelset(sigset_t *set, int signum);	将某个信号清出信号集
int sigismenber(const sigset_t *set, int signum);	判断某个信号是否在信号集中。
```

**sigpromask函数：**

用来屏蔽信号、解除屏蔽。其本质，读取或修改进程的信号屏蔽字（PCB中）;

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

参数：

- set：传入参数，是一个位图，set中哪个位置1，就表示屏蔽哪个信号。
- oldset：传出参数，保存旧的信号屏蔽集。
- how：参数取值：假设当前信号屏蔽字为mask。
  - SIG_BLOCK：set表示将要屏蔽的信号。相当于mask=mask|set。
  - SIG_UNBLOCK：set表示需要解除屏蔽的信号。相当于mask=mask&~set。
  - SIG_SETMASK：set表示用于替代原始屏蔽集的新屏蔽集。

**sigpending函数：**

读取当前进程的未决信号集

```c
int sigpending(sigset_t *set);	//set传出参数。
```

练习：编写程序。把所有常规信号的未决状态打印至屏幕。

# 7 线程

## 7.2 线程同步

线程同步，指一个线程发出某一功能调用时，在没有得到结果前，该调用不返回。同时其他进程为保证数据一致性，不能调用该功能。

**线程同步例子**

创建两个子线程，让两个线程共享一个全局变量int num，然后让每个线程数5000词



# 8 tcp/ip协议

## 8.1 常见协议

- 传输层：TCP/UDP协议
- 应用层：HTTP协议，FTP协议。
- 网络层：IP协议，ICMP协议，IGMP协议。
- 网络接口层：ARP协议，RARP协议。

TCP[传输控制协议](http://baike.baidu.com/view/544903.htm)（Transmission Control Protocol）是一种面向连接的、可靠的、基于字节流的[传输层](http://baike.baidu.com/view/239605.htm)通信协议。

UDP用户数据报协议（User Datagram Protocol）是[OSI](http://baike.baidu.com/view/113948.htm)参考模型中一种无连接的[传输层](http://baike.baidu.com/view/239605.htm)协议，提供面向事务的简单不可靠信息传送服务。

HTTP[超文本传输协议](http://baike.baidu.com/view/468465.htm)（Hyper Text Transfer Protocol）是[互联网](http://baike.baidu.com/view/6825.htm)上应用最为广泛的一种[网络协议](http://baike.baidu.com/view/16603.htm)。

FTP文件传输协议（File Transfer Protocol）

IP协议是[因特网](http://baike.baidu.com/view/1706.htm)互联协议（Internet Protocol）

ICMP协议是Internet控制[报文](http://baike.baidu.com/view/175122.htm)协议（Internet Control Message Protocol）它是[TCP/IP协议族](http://baike.baidu.com/view/2221037.htm)的一个子协议，用于在IP[主机](http://baike.baidu.com/view/23880.htm)、[路由](http://baike.baidu.com/view/18655.htm)器之间传递控制消息。

IGMP协议是 Internet 组管理协议（Internet Group Management Protocol），是因特网协议家族中的一个组播协议。该协议运行在主机和组播路由器之间。

[ARP](http://baike.baidu.com/view/32698.htm)协议是正向[地址解析协议](http://baike.baidu.com/view/149421.htm)（Address Resolution Protocol），通过已知的IP，寻找对应主机的[MAC地址](http://baike.baidu.com/view/69334.htm)。

[RARP](http://baike.baidu.com/view/32772.htm)是反向地址转换协议，通过MAC地址确定IP地址。

## 8.2 分层模型

OSI是Open System Interconnection的缩写, 意为开放式系统互联. [国际标准化组织](https://baike.so.com/doc/5340499-5575942.html)(ISO)制定了OSI模型, 该模型定义了不同计算机互联的标准, 是设计和描述计算机网络通信的基本框架.

网络分层 OSI 7层模型: 物数网传会表应 

Ø 物理层---双绞线，光纤（传输介质），将模拟信号转换为数字信号

Ø 数据链路层---数据校验，定义了网络传输的基本单位-帧 

Ø 网络层---定义网络，两台机器之间传输的路径选择点到点的传输

Ø 传输层---传输数据 TCP，UDP，端到端的传输 

Ø 会话层---通过传输层建立数据传输的通道. 

Ø 表示层---编解码，翻译工作.

Ø 应用层---为客户提供各种应用服务，email服务，ftp服务，ssh服务 



**物数网传会表应**

![1](D:\work_station\黑马c++\git\linux_study\pic\1.png)

![2](D:\work_station\黑马c++\git\linux_study\pic\2.png)

![3](D:\work_station\黑马c++\git\linux_study\pic\3.png)



通过IP地址来确定网络环境中的唯一的一台主机;

主机上使用端口号来区分不同的应用程序.

IP+端口唯一确定唯一一台主机上的一个应用程序.

 

## 8.3 socket通信编程

发送端：数据层层打包。

接收端：数据层层解包。

### 8.3.1 服务端开发流程

- 创建soket，返回一个文件描述符lfd = socket()。---该文件描述符用于监听客户端链接。
- 将lfd和IP PORT进行绑定---bind();
- 将lfd有主动变为被动监听---listen();
- 接受一个新的连接，得到一个文件描述符cfd---accept();---该文件描述符用于和客户端进行通信的。
- while(1){ 接受数据(read/recv)； 发送数据(write/send)； }
- 关闭文件描述符---close(lfd),close(cfd);



### 8.3.2 客户端开发流程

- 创建socket，返回一个文件描述符cfd；
- 连接服务器---connect();
- while(1){ //发送数据---write或者send  //接收数据---read或者recv}
- close(cfd);



## 8.4 三次握手四次挥手



思考：为什么TCP是面向连接

**三次握手：建立连接需要三次握手。**

1、-> SYN, 2000(0), <mss 1024>

2、<- SYN, 5000(0), ACK(2001), <mss 1024>

3、-> ACK(5001);

**数据传输过程**

4、-> seq: 2001(20);

5、<- seq: 5001(50), ACK(2021)

6、-> ACK(5051)

**四次挥手：断开连接需要四次挥手。**

7、  -> FIN  (2021), 

8、  <- ACK (2022)

9、  <- FIN  (5051)

10、-> ACK (5052)



# 9 git操作相关

```c++
git add . 
git commit -m "first commit" 
git remote rm origin 
git remote add origin .git 
git branch -M main               
git push -u origin main  

// 本地代码未修改，只有master分支，直接更新
git pull
```

  



# 附录

## 进程间通信方式 

  （1）. 管道pipe：管道是一种半双工的通信方式，数据只能单向流动，而且只能在具有亲缘关系的进程间使用。进程的亲缘关系通常是指父子进程关系。 
  （2）. 命名管道FIFO：有名管道也是半双工的通信方式，但是它允许无亲缘关系进程间的通信。 
  （3）消息队列MessageQueue：消息队列是由消息的[链表]()，存放在内核中并由消息队列标识符标识。消息队列克服了信号传递信息少、管道只能承载无格式字节流以及缓冲区大小受限等缺点。 
  （4）共享存储SharedMemory：共享内存就是映射一段能被其他进程所访问的内存，这段共享内存由一个进程创建，但多个进程都可以访问。共享内存是最快的 IPC 方式，它是针对其他进程间通信方式运行效率低而专门设计的。它往往与其他通信机制，如信号两，配合使用，来实现进程间的同步和通信。 
  （5）信号量Semaphore：信号量是一个计数器，可以用来控制多个进程对共享资源的访问。它常作为一种锁机制，防止某进程正在访问共享资源时，其他进程也访问该资源。因此，主要作为进程间以及同一进程内不同线程之间的同步手段。 
  （6） 套接字Socket：套解口也是一种进程间通信机制，与其他通信机制不同的是，它可用于不同及其间的进程通信。 
  （7）信号 ( sinal ) ： 信号是一种比较复杂的通信方式，用于通知接收进程某个事件已经发生。



