//服务端程序
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

int main()
{
	//创建socket
	int cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd < 0)
	{
		perror("error\n");
		return -1;
	}

	//绑定
	struct sockaddr_in serv;
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);
	printf("ip == [%x]\n", serv.sin_addr.s_addr);
	
	int ret = connect(cfd,(struct sockaddr *) &serv, sizeof(serv));
	if(ret < 0)
	{
		perror("error\n");
		return -1;
	}
	
	int n = 0;
	char buf[256];
	while(1)
	{
		//都标准输入数据
		memset(buf, 0x00, sizeof(buf));
		n = read(STDIN_FILENO, buf, sizeof(buf));
		
		//发数据
		write(cfd, buf, n);

		//读服务端发送来的数据
		memset(buf, 0x00, sizeof(buf));
		n = read(cfd, buf, sizeof(buf));
		if(n <=0)
		{
			perror("server close\n");
		}
		printf("size == [%d],recv data == %s", n, buf);
	}

	close(cfd);

	return 0;
}
