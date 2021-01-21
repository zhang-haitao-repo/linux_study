gcc -c fun1.c fun2.c

ar rcs libtest1.a fun1.o fun2.o

gcc -o main1 main.c -I./include -L./lib -ltest1