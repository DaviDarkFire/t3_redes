#include "xarp.h"

char* build_xarp_show_message(){
  char* message;
  message = malloc(1);
  strcpy(message, (char*)XARP_SHOW); // opcode only
  return message;
}

char* build_xarp_res_message(char** args){
  char* message;
  message = malloc(5);
  strcpy(message, (char*)XARP_DEL);
  strcat(message, args[2]); // sends the desired ip address along with the opcode
  return message;
}

char* build_xarp_add_message(char** args){
  
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
        // message = build_xarp_add_message(argv);
      }
      else{
        if(strcmp(op, "del") == 0){
          // message = build_xarp_add_message(args);
        }
        else{
          if(strcmp(op, "ttl") == 0){
            // message = build_xarp_res_message();
          }
        }
      }
    }
  }
  // int sockfd = client_create_socket();
  // client_send_request(sockfd, message);
  // free(message); // TODO: esse free fica aqui msm????
}
