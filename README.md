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

-c  只编译, 生成.o文件, 通常称为目标文件

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

# 5 进程间通信

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

## 5.2 管道

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





========================================================================== <br>

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

​                                         