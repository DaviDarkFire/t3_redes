#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
  int ip_address;
  unsigned char eth_address[6];
  int ttl;
  struct node* next;
} node_t;

node_t* add_node(node_t* current_last_node, int ip_address, unsigned char eth_address[6], int ttl){
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
  printf("Entrada\t\tEndereço IP\t\tEndereço Ethernet\t\tTTL\n");
  for(i = 0; current != NULL; i++){
    // TODO: printar direito os endereços
    printf("%d\t\t%d\t\t\t%02X:%02X:%02X:%02X:%02X:%02X\t\t%d\n",
    i, current->ip_address, current->eth_address[0], current->eth_address[1],
    current->eth_address[2], current->eth_address[3], current->eth_address[4],
    current->eth_address[5], current->ttl);
    current = current->next;
  }
}

// modifies dest_array with 6 separate bytes on each array element given a string with format XX:XX:XX:XX:XX:XX
void get_eth_addr_as_6_bytes_from_string(unsigned char dest_array[6], char* src_str){
    int i_aux[6];

    sscanf(src_str, "%x:%x:%x:%x:%x:%x",
    &i_aux[0], &i_aux[1], &i_aux[2],
    &i_aux[3], &i_aux[4], &i_aux[5]);

    dest_array[0] = (unsigned char) i_aux[0];
    dest_array[1] = (unsigned char) i_aux[1];
    dest_array[2] = (unsigned char) i_aux[2];
    dest_array[3] = (unsigned char) i_aux[3];
    dest_array[4] = (unsigned char) i_aux[4];
    dest_array[5] = (unsigned char) i_aux[5];
}

// returns desired node if it exists in the list, otherwise, returns NULL (be careful!)
node_t* find_node_by_ip_address(node_t* head, int desired_ip_addr){
    node_t* current = head;
    while(current != NULL){
      if(current->ip_address == desired_ip_addr)
        return current;
      current = current->next;
    }
    return NULL;
}

int main(int argc, char** argv){
    unsigned char c_eth_addr[6];

    get_eth_addr_as_6_bytes_from_string(c_eth_addr ,argv[1]);

    // printf("char: %02X:%02X:%02X:%02X:%02X:%02X\n",
    // c_eth_addr[0], c_eth_addr[1],
    // c_eth_addr[2], c_eth_addr[3],
    // c_eth_addr[4], c_eth_addr[5]); // DEBUG

    node_t* first_node = add_node(NULL, 0, c_eth_addr, 0);
    node_t* aux = first_node;
    node_t* node;

    for (int i = 1; i < 10; i++){
        node = add_node(first_node, i, c_eth_addr, i);
        first_node = node;
    }
    node_t* find_ip_5 = find_node_by_ip_address(aux, 12);
    if(find_ip_5 != NULL){
        printf("Achei o nó com ip: %d\n", find_ip_5->ip_address);
    } else {
      printf("achei nulo\n");
    }


    // delete_node_by_ip_address(&aux,0); // DEBUG
    print_list(aux);

}
