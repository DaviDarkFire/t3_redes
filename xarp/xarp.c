#include "xarp.h"

// xarp show
char* build_xarp_show_message(){
  char* message;
  message = malloc(1);
  strcpy(message, (char*)XARP_SHOW); // opcode only
  return message;
}

// xarp res EndereçoIP
char* build_xarp_res_message(char** args){
  char* message;
  char* ip_bytes;

  message = malloc(5); // 1 + 4

  ip_bytes = get_ip_addr_bytes_from_string(args[2]);

  strcpy(message, (char*)XARP_RES); // opcodde
  strcat(message, ip_bytes); // ip_address
  return message;
}

char* get_mac_addr_bytes_from_string(char* colon_format_mac){
  char* mac_bytes;
  int i;
  unsigned int i_aux[6];
  mac_bytes = malloc(sizeof(char) * 6);
  sscanf(colon_format_mac, "%X:%X:%X:%X:%X:%X",
	&i_aux[0], &i_aux[1], &i_aux[2],
	&i_aux[3], &i_aux[4], &i_aux[5]);

  for(i = 0; i < 6; i++){
    mac_bytes[i] = (char) i_aux[i];
  }

  return mac_bytes;
}

char* get_ip_addr_bytes_from_string(char* dotted_dec_ip){
  unsigned int values[4];
  int i;
  char* ip_bytes;
  ip_bytes = malloc(4);

  sscanf(dotted_dec_ip, "%u.%u.%u.%u",
  &values[0], &values[1], &values[2], &values[3]);

  for(i = 0; i < 4; i++){
    ip_bytes[i] = (char) values[i];
  }
  return ip_bytes;
}

//xarp add EndereçoIP EndereçoEthernet ttl
char* build_xarp_add_message(char** args){
  char* mac_bytes;
  char* message;
  char* ip_bytes;

  message = malloc(15); // 1 + 4 + 6 + 4

  mac_bytes = get_mac_addr_bytes_from_string(args[3]);
  ip_bytes = get_ip_addr_bytes_from_string(args[2]);

  strcpy(message, (char*)XARP_ADD); // opcode
  strcat(message, ip_bytes); // ip address
  strcat(message, mac_bytes); // ethernet address as 6 bytes
  strcat(message, args[4]); // ttl

  free(ip_bytes);
  free(mac_bytes);

  return message;
}

// xarp del endereçoIP
char* build_xarp_del_message(char** args){
  char* message;
  char* ip_bytes;

  message = malloc(5); // 1 + 4

  ip_bytes = get_ip_addr_bytes_from_string(args[2]);

  strcpy(message, (char*) XARP_DEL);
  strcat(message, ip_bytes);

  free(ip_bytes);

  return message;
}

char* get_ttl_bytes_from_string(int i_ttl){
  char* ttl;
  ttl = malloc(4);

  ttl[0] = i_ttl >> 24;
  ttl[1] = i_ttl >> 16;
  ttl[2] = i_ttl >> 8;
  ttl[3] = i_ttl >> 0;

  return ttl;
}

// xarp ttl <number>
char* build_xarp_ttl_message(char* ttl){
  char* message;
  int aux;
  char* ttl_bytes;

  message = malloc(5); // 1 + 4

  aux = atoi(ttl);
  ttl_bytes = get_ttl_bytes_from_string(aux);

  strcpy(message, (char*) XARP_TTL);
  strcat(message, ttl_bytes);

  free(ttl_bytes);

  return message;
}

int main(int argc, char** argv){
  char* op = argv[1];
  char* message;

  if(strcmp(op, "show") == 0){
    message = build_xarp_show_message();
  }
  else{
    if(strcmp(op, "res") == 0){
      message = build_xarp_res_message(argv);
    }
    else{
      if(strcmp(op, "add") == 0){
        message = build_xarp_add_message(argv);
      }
      else{
        if(strcmp(op, "del") == 0){
          message = build_xarp_del_message(argv);
        }
        else{
          if(strcmp(op, "ttl") == 0){
            message = build_xarp_ttl_message(argv[2]);
          }
        }
      }
    }
  }
  // int sockfd = client_create_socket();
  // client_send_request(sockfd, message);
  // free(message); // TODO: esse free fica aqui msm????
}
