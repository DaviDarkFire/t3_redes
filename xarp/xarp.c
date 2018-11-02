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
  message = malloc(5); // 1 + 4
  strcpy(message, (char*)XARP_RES);
  strcat(message, args[2]); // sends the desired ip address along with the opcode
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

//xarp add EndereçoIP EndereçoEthernet ttl
char* build_xarp_add_message(char** args){
  char* mac_bytes;
  char* message;
  message = malloc(15); // 1 + 4 + 6 + 4
  mac_bytes = get_mac_addr_bytes_from_string(args[3]);
  strcpy(message, (char*)XARP_ADD);
  strcat(message, args[2]);
  strcat(message, mac_bytes);
  strcat(message, args[4]);
  free(mac_bytes);
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
          // message = build_xarp_del_message(args);
        }
        else{
          if(strcmp(op, "ttl") == 0){
            // message = build_xarp_ttl_message();
          }
        }
      }
    }
  }
  // int sockfd = client_create_socket();
  // client_send_request(sockfd, message);
  // free(message); // TODO: esse free fica aqui msm????
}
