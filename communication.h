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
	#include "linked_list.h"

	#define PORT 5050
	#define DEFAULT_IP "127.0.0.1"

	#define XARP_SHOW 0
	#define XARP_RES 1
	#define XARP_ADD 2
	#define XARP_DEL 3
	#define XARP_TTL 4
	#define XIFCONFIG_INFO 5
  #define XIFCONFIG_IP 6
  #define XIFCONFIG_MTU 7

	int arp_socket_creation();
	int arp_get_connection(int socket);
	char* arp_get_request(int connfd);
	int arp_send_response(int connfd, char* buffer);
	int client_create_socket();
	void client_send_request(int socket, char* buffer);
	char* client_get_response(int connfd);
	int arp_send_response(int connfd, char* buffer);
	char* get_ip_addr_bytes_from_string(char* dotted_dec_ip);
	void printBits(size_t const size, void const * const ptr);
#endif
