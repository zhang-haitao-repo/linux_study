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



==========================================================================
git add .
git commit -m "first commit"
git remote rm origin
git remote add origin .git
git branch -M main                                                       
git push -u origin main                                                 