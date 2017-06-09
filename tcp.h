
int tcp_init(int port);

int tcp_server_init(int tcp_fd);

int tcp_client_init(int tcp_fd, int server_port, char *server_ip);

int tcp_send(int tcp_fd, char *msg, int len);

int tcp_recv(int tcp_fd, char *msg, int len);


