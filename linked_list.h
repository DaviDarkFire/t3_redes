#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
  unsigned int ip_address;
  unsigned char eth_address[6];
  unsigned int ttl;
  struct node* next;
} node_t;