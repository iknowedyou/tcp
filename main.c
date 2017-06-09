#include "tcp.h"

#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: ./%s <self_port> \n(if self_port==0 mean self is server,else is clent)\n", argv[0]);
		return -1;
	}
	int input_port;
	
	
	input_port = atoi(argv[1]);
	if(input_port == 0)
	{
			//server
		int server_fd;
		
		server_fd = tcp_init(12345);
		
		tcp_server_init(server_fd);
		
		return 0;
	}
	else
	{
			//client
		int client_fd;
		
		char recv_msg[1024];
		
		int my_port = input_port;
		int server_port = 12345;
		char server_ip[] = "192.168.38.128";
		
		client_fd = tcp_init(my_port);
		
		tcp_client_init(client_fd, server_port, server_ip);
		
		while(1)
		{
			bzero(recv_msg, 1024);
			if(tcp_recv(client_fd, recv_msg, 1024) <= 0)
			{
				close(client_fd);
				client_fd = tcp_init(my_port);
				while(tcp_client_init(client_fd, server_port, server_ip) < 0)
				{
					sleep(1);
				}
			}
		}
		
		return 0;
	}
}