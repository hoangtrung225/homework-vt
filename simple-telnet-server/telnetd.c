#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#define DEFAULT_PORT "2323"
#define BACKLOG 10
#define BUFFSIZE 1024 //* 64
int main(int argc, char** argv){
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    int sockfd, new_fd;
	int byte;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, DEFAULT_PORT, &hints, &res) != 0){
        printf("Error: server fail to get address information\n ");
        return 0;
    }

    //make socket from info get from system
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);
    
    printf("server: waiting for connections...\n");
    
    //accept incoming connection
    addr_size = sizeof client_addr;
    new_fd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size); 
	struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
	char ipstr[14];
	int port;
	printf("Connect from socket: %d", new_fd);

	if(new_fd < 0){
		printf("Error: server fail to establis connection\n");
		return 0;	
	}
	else{
	    port = ntohs(s->sin_port);
	    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		printf("Accept connection from %s:%d:", ipstr, port);
	}
	char buffer[BUFFSIZE];
	while(1){
		byte = send_shell_header(new_fd, buffer);
		if(byte < 0){
			printf("Error: connection lost!\n");
			close(new_fd);
			return 0;
		}
		byte = recv(new_fd, buffer, BUFFSIZE, 0);
		if(byte < 0){
			printf("Error: connection lost!\n");
			close(new_fd);
			return 0;
		}
		byte = send(new_fd, buffer, BUFFSIZE, 0);
	}
}

int message_send_all(int sock, char* buffer, int buff_length, int flag){
	int send_data = 0;
	int packet_send = 0;
	while(send_data < buff_length){
		packet_send = send(sock, buffer + send_data, buff_length - send_data, flag);
		send_data = send_data + packet_send;
	}
	return send_data;
}

int send_shell_header(int sock, char* buffer){
	if(gethostname(buffer, BUFFSIZE) != 0){
		printf("Error: system failure\n");
		sprintf(buffer, "Error: system failure\n");
		send(sock, buffer, BUFFSIZE, 0);
		return 0;
	}
	int buff_len = strlen(buffer);
	buffer[buff_len] = '@';
	if(getcwd(buffer + buff_len + 1, BUFFSIZE - buff_len - 1) == NULL){
		printf("Error: system failure\n");
		sprintf(buffer, "Error: system failure\n");
		send(sock, buffer, BUFFSIZE, 0);
		return 0;
	}
	int i = send(sock, buffer, strlen(buffer), 0);
	return i;
}
