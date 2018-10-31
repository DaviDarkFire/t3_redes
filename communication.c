#include "communication.h"
#define BUFFSIZE 1024


int arp_socket_creation(){ //criando o socket no qual o protocolo arp vai responder os outros programas
	int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serv_addr;

	if(sockfd < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	memset((char*) &serv_addr, 0, sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(DEFFAULT_IP);
	serv_addr.sin_port = htons(PORT);

	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) { //associa socket a porta que será usada
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}
	return sockfd;


}

int arp_get_connection(int sockfd){
	socklen_t clilen; //tamanho do cliente
	struct sockaddr_in cli_addr; //estrutura do cliente
	int connfd; //file descriptor pra cada conexão de cliente

	clilen = sizeof(cli_addr);

	while(1) { //loop de conexão
		memset((char*) &cli_addr, 0, sizeof(cli_addr)); //zera o serv_addr
		connfd = accept(sockfd, (struct sockaddr*) &cli_addr, (unsigned int*) &clilen);//tenta nova conexão
		if(connfd < 0) { //verifica erro
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}
		close(sockfd);
		return connfd;
	}
}

char* arp_get_request(int connfd){
	char* buffer = malloc(sizeof(char)*BUFFSIZE);
	memset((char*) buffer, 0, BUFFSIZE);
	int rec_bytes = 0;
	int n = 0;
	do{
		rec_bytes = recv(connfd, buffer+n, BUFFSIZE-n, 0);
		n += rec_bytes;
	}while(rec_bytes != 0);

	return buffer;
}

int arp_send_response(int connfd, char* buffer){
	int n = strlen(buffer);
	int m = send(connfd, buffer,n, 0);
	while(m < n) {
		m += send(connfd, buffer + m, n - m, 0);
	}
	return 0;
}

int client_create_socket(){
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
	if(sockfd < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	return sockfd;


}
void client_send_request(int sockfd, char* buffer){

	struct sockaddr_in serv_addr;

  	memset((char*) &serv_addr, 0, sizeof(serv_addr));
  
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	
	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	int bytes_read = 0;
	int bytes_sent = send(sockfd, buffer, bytes_read, 0);
	while(bytes_sent < bytes_read){
		bytes_sent += send(sockfd, buffer + bytes_sent, bytes_read - bytes_sent, 0);
	}
}

char* client_get_response(int connfd){
	char buffer[BUFFSIZE];
	int n = 0;
	int bytes_received = 0;
	memset(buffer, 0, sizeof(buffer));

	do{ 
			bytes_received = recv(connfd, buffer+n, sizeof(buffer)-n, 0);
			n += bytes_received;

	  }while(bytes_received != 0);

	close(connfd);
	return 0;


}

// int main(void){

// 	return 0;
// }