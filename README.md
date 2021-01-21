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

### 1.2.1 库的介绍

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

