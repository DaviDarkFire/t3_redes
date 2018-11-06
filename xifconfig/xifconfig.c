#include "xifconfig.h"

unsigned int decide_mode(int argc, char** argv){
  if (argc == 1)
    return DEFAULT_MODE;
  if(strcmp(argv[2], "mtu") == 0)
    return SET_MTU_MODE;
  return CONFIG_IP_MODE;
}

void set_mtu_mode(char* iface, char* new_mtu){
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  struct ifreq ifr;

  strcpy(ifr.ifr_name, iface);

  ifr.ifr_mtu = atoi(new_mtu);
  if(!ioctl(sockfd, SIOCSIFMTU, &ifr)) {
    printf("MTU changed successfully.\n");
  }
}

void configure_ip_mode(char* iface, char* ip_addr, char* netmask){
    struct ifreq ifr;
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strncpy(ifr.ifr_name, iface, IFNAMSIZ); //IFNAMSIZ is the iface's max name length
    ifr.ifr_addr.sa_family = AF_INET;
    inet_pton(AF_INET, ip_addr, ifr.ifr_addr.sa_data + 2);
    ioctl(sockfd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, netmask, ifr.ifr_addr.sa_data + 2);
    ioctl(sockfd, SIOCSIFNETMASK, &ifr);

    ioctl(sockfd, SIOCGIFFLAGS, &ifr);
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

    ioctl(sockfd, SIOCSIFFLAGS, &ifr);
}

char* build_xifconfig_info_message(){
  char* message;
  char opcode[1];

  message = malloc(sizeof(char)*1); //

  sprintf(opcode, "%d", XIFCONFIG_INFO);
  memcpy(message, opcode, sizeof(char)); // opcode
  return message;
}

// xifconfig <interface> <IP address> <IP Netmask>
char* build_xifconfig_ip_message(char** args){
  char* message;
  char opcode[1];
  char* ip_bytes;
  char* netmask;

  message = malloc(sizeof(char)*31); // 1 + 22 + 4 + 4
  ip_bytes = get_ip_addr_bytes_from_string(args[2]);
  netmask = get_ip_addr_bytes_from_string(args[3]); // netmask is in the same format as an ip address

  printf("args[2] na build: %s\n", args[2]); // DEBUG
  printf("args[3] na build: %s\n", args[3]); // DEBUG
  printf("ip_bytes na build: %s\n", ip_bytes); // DEBUG
  printf("netmask na build: %s\n", netmask); // DEBUG

  printBits(sizeof(ip_bytes), ip_bytes); // DEBUG
  printBits(sizeof(netmask), netmask); // DEBUG

  // printf("strlen(args[1]): %d\n", (int)strlen(args[1])); // DEBUG
  // char stuffed_ifname[6];
  // int i;
  // for(i = 0; i < 6; i++){
  //   if(i < strlen(args[1]))
  //     stuffed_ifname[i] = args[1][i];
  //   else
  //     stuffed_ifname[i] = '$';
  // }
  // printf("stuffed_ifname: %s", stuffed_ifname); // DEBUG

  sprintf(opcode, "%d", XIFCONFIG_IP);
  memcpy(message, opcode, sizeof(char)); // opcode
  memcpy(message+1, args[1]/*stuffed_ifname*/, 6); // ifname
  memcpy(message+1+6, ip_bytes, 4);
  memcpy(message+1+6+4, netmask, 4);

  free(ip_bytes);
  free(netmask);
  printf("message na build build_xifconfig_ip_message: %s\n", message); // DEBUG
  return message;
}

int main(int argc, char** argv){
  unsigned int mode = decide_mode(argc, argv);
  char* message;
  switch(mode){
    case DEFAULT_MODE:{
      message = build_xifconfig_info_message();
      printf("%s\n", message); // DEBUG
      // int sockfd = client_create_socket();
      // client_send_request(sockfd, message);
      // char* resp = client_get_response();
      printf("You chose default mode.\n"); //DEBUG
      break;
    }

    case CONFIG_IP_MODE:
      message = build_xifconfig_ip_message(argv);
      printf("%s &&&\n", message); // DEBUG
      printf("You chose ip configuration mode.\n"); // DEBUG
      configure_ip_mode(argv[1], argv[2], argv[3]);
      // printf("Interface's IP is configured as desired.\n"); // DEBUG
    break;

    case SET_MTU_MODE:
      printf("You chose MTU mode.\n"); //DEBUG
      set_mtu_mode(argv[1], argv[3]);
    break;

    default:
      printf("Couldn't understand desired operation.\n");
    break;
  }
  return 0;
}
