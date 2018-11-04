#include "xarp.h"

// xarp show
char* build_xarp_show_message(){
  char* message;
  char opcode[1];
  message = malloc(sizeof(char)*1);
  sprintf(opcode, "%d", XARP_SHOW);
  memcpy(message, opcode, sizeof(char)); // opcode
  return message;
}

// xarp res EndereçoIP
char* build_xarp_res_message(char** args){
  char* message;
  char* ip_bytes;
  char opcode[1];

  message = malloc(sizeof(char)*5); // 1 + 4

  ip_bytes = get_ip_addr_bytes_from_string(args[2]);

  sprintf(opcode, "%d", XARP_RES);
  memcpy(message, opcode, sizeof(char)); // opcode
  memcpy(message+1, ip_bytes, 4); // ip address
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
  char* ip_bytes;
  char* ttl_bytes;
  char* message;
  char opcode[1];

  message = malloc(15); // 1 + 4 + 6 + 4

  mac_bytes = get_mac_addr_bytes_from_string(args[3]);
  ip_bytes = get_ip_addr_bytes_from_string(args[2]);
  ttl_bytes = get_ttl_bytes_from_string(args[4]);

  sprintf(opcode, "%d", XARP_ADD);
  memcpy(message, opcode, sizeof(char)); // opcode
  memcpy(message+1, ip_bytes, 4); // ip address
  memcpy(message+1+4, mac_bytes, 6); // ethernet address as 6 bytes
  memcpy(message+1+4+6, ttl_bytes, 4); //ttl

  free(ip_bytes);
  free(mac_bytes);
  free(ttl_bytes);

  return message;
}

// xarp del endereçoIP
char* build_xarp_del_message(char** args){
  char* message;
  char* ip_bytes;
  char opcode[1];

  message = malloc(5); // 1 + 4

  ip_bytes = get_ip_addr_bytes_from_string(args[2]);
  sprintf(opcode, "%d", XARP_DEL);
  memcpy(message, opcode, sizeof(char)); // opcode
  memcpy(message+1, ip_bytes, 4); // ip address

  free(ip_bytes);

  return message;
}

char* get_ttl_bytes_from_string(char* str_ttl){
  char* ttl_bytes;
  int i_ttl;
  i_ttl = atoi(str_ttl);
  ttl_bytes = malloc(sizeof(char)*4);

  ttl_bytes[0] = i_ttl >> 24;
  ttl_bytes[1] = i_ttl >> 16;
  ttl_bytes[2] = i_ttl >> 8;
  ttl_bytes[3] = i_ttl >> 0;

  return ttl_bytes;
}

// xarp ttl <number>
char* build_xarp_ttl_message(char* ttl){
  char* message = malloc(5*sizeof(char));
  char* ttl_bytes = NULL;
  char opcode[1];

  ttl_bytes = get_ttl_bytes_from_string(ttl);

  sprintf(opcode, "%d", XARP_TTL);
  memcpy(message, opcode, sizeof(char)); // opcode
  memcpy(message+1, ttl_bytes, 4); // ttl

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

  printf("message: %s\n", message); //DEBUG

  int sockfd, bytes_received, total_bytes_received;
	char buffer[BUFFSIZE];
	struct sockaddr_in serv_addr;

  sprintf(buffer, "%s\0", message);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	memset((char*) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	serv_addr.sin_port = htons(PORT);

	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	if(send(sockfd, buffer, strlen(buffer), 0) < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	memset(buffer, 0, sizeof(buffer));

  total_bytes_received = 0;
  do{
      bytes_received = recv(sockfd, buffer+total_bytes_received, BUFFSIZE-total_bytes_received, 0);
			total_bytes_received += bytes_received;
	}while(bytes_received > 0);

	if(bytes_received < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	printf("Mensagem recebida:\n%s\n", buffer);

  free(message);
  return 0;
}
