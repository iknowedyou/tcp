#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "tcp.h"

int tcp_init(int port)
{
	int tcp_fd;
	
	struct sockaddr_in self;
	int len = sizeof(self);
	bzero(&self, len);
	
	tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_fd < 0)
		return tcp_fd;
	
	self.sin_family = AF_INET;
	self.sin_port = htons(port);
	self.sin_addr.s_addr = 0;
	
	if(bind(tcp_fd, (struct sockaddr *)&self, len) < 0)
	{
		perror("bind");
		return -1;
	}	
	
	return tcp_fd;
}

int tcp_server_init(int tcp_fd)
{
	struct sockaddr_in peer;
	int len = sizeof(peer);
	int peer_fd;
	int pid;
	
	listen(tcp_fd, 10);
	
	signal(SIGPIPE, SIG_IGN);
	
	while(1)
	{
		peer_fd = accept(tcp_fd, (struct sockaddr *)&peer, &len);
		if(peer_fd < 0)
		{
			perror("accept");
			return -1;
		}
		
		pid = fork();
		
		if(pid > 0)
			continue;
		else if(pid < 0)
			perror("fork");
		else
		{
			while(1)
			{
				if(tcp_send(peer_fd, "The msg is from server!", 100) < 0)
					return -1;
				sleep(2);
			}
		}
	}
}

int tcp_client_init(int tcp_fd, int server_port, char *server_ip)
{
	int ret;
	struct sockaddr_in server;
	int len = sizeof(server);
	bzero(&server, len);
	
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	server.sin_addr.s_addr = inet_addr(server_ip);
	
	ret = connect(tcp_fd, (struct sockaddr *)&server, len);
	
	if(ret < 0)
	{
		perror("connect");
		return ret;
	}
}

int tcp_send(int tcp_fd, char *msg, int len)
{
	if(send(tcp_fd, msg, len, 0) < 0)
	{
		perror("send");
		return -1;
	}
	return 0;
}

int tcp_recv(int tcp_fd, char *msg, int len)
{
	int ret;
	ret = recv(tcp_fd, msg, len, 0);
	
	if(ret < 0)
	{
		perror("recv");
	}
	else if(ret == 0)
	{
		printf("Don't recv msg or peer shutdown");
	}
	else
		printf("len:%d,msg:%s\n", ret, msg);
	
	return ret;
}


