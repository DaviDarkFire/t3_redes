#ifndef __COMMUNICATION__
#define __COMMUNICATION__
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/socket.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>

	#define PORT 5050
	#define DEFFAULT_IP "127.0.0.1"

	#define SHOW 0
	#define RES 1
	#define ADD 2
	#define DEL 3
	#define TTL 4
	#define ETHERNET_INFO 5

	int arp_socket_creation();
	int arp_get_connection(int socket);
	char* arp_get_request(int connfd);
	int arp_send_response(int connfd, char* buffer);
	int client_create_socket();
	void client_send_request(int socket, char* buffer);
	char* client_get_response(int connfd);
	int arp_send_response(int connfd, char* buffer);
#endif