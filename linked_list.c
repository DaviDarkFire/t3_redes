#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
  int ip_address;
  char eth_address[6];
  int ttl;
  struct node* next;
} node_t;

node_t* add_node(node_t* current_last_node, int ip_address, char eth_address[6], int ttl){
    node_t* new_node;
    new_node = (node_t*) malloc(sizeof(node_t));
    new_node->ip_address = ip_address;
    // printf("TAMANHO eth_address: %s\n", eth_address);
    // printf("eth_address dentro da addnode: %c %c %c %c %c %c\n", eth_address[0], eth_address[1],
    //     eth_address[2], eth_address[3], eth_address[4], eth_address[5]);
    strncpy(new_node->eth_address, eth_address, 6);
    new_node->ttl = ttl;
    new_node->next = NULL;
    if(current_last_node != NULL)
        current_last_node->next = new_node;
    return new_node;
}

int delete_node_by_ip_address(node_t** head, int del_ip_addr){
  int i;
  int retval = -1;
  node_t* current = *head;
  node_t* previous = NULL;

  //exclusão da head
  if(del_ip_addr == (*head)->ip_address){
    retval = del_ip_addr;
    node_t* aux = *head;
    *head = (*head)->next;
    free(aux);
    return retval;
  }

  while(1){
    if(current == NULL)
      return -1;
    if(current->ip_address == del_ip_addr) // current is the node we'll delete
      break;
    previous = current;
    current = current->next;
  }
  if(previous != NULL)
    previous->next = current->next;
  retval = current->ip_address;
  free(current);
  return retval;
}

void print_list(node_t * head){
  node_t * current = head;
  int i;
  printf("Entrada\tEndereço IP\t\t\tEndereço Ethernet\t\tTTL\n");
  for(i = 0; current != NULL; i++){
    // TODO: printar direito os endereços
    printf("%d\t\t%d\t\t\t%02X:%02X:%02X:%02X:%02X:%02X\t\t%d\n",
    i, current->ip_address, current->eth_address[0], current->eth_address[1],
    current->eth_address[2], current->eth_address[3], current->eth_address[4],
    current->eth_address[5], current->ttl);
    current = current->next;
  }
}

int main(int argc, char** argv){
    int eth_addr[6];

    sscanf(argv[1], "%x:%x:%x:%x:%x:%x",
    &eth_addr[0], &eth_addr[1], &eth_addr[2],
    &eth_addr[3], &eth_addr[4], &eth_addr[5]);

    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
    eth_addr[0], eth_addr[1],
    eth_addr[2], eth_addr[3],
    eth_addr[4], eth_addr[5]);

    node_t* first_node = add_node(NULL, 0, "000000", 0);
    node_t* aux = first_node;
    node_t* node;
    char* string_teste = "123456";
    // strcpy(string_teste,"123456");
    // printf("string_teste: %d %d %d %d %d %d\n", string_teste[0], string_teste[1],
    //     string_teste[2], string_teste[3], string_teste[4], string_teste[5]);

    // printf("string_test: %s\n", string_teste);
    for (int i = 1; i < 10; i++){
        node = add_node(first_node, i, string_teste, i);
        first_node = node;
    }
    delete_node_by_ip_address(&aux,9);
    print_list(aux);

}
