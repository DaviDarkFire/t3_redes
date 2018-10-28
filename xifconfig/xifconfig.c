#include "defines.h"

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
  if(!ioctl(sockfd, SIOCGIFMTU, &ifr)) {
    // ifr.ifr_mtu // Contains current mtu value
  }
  ifr.ifr_mtu = atoi(new_mtu);
  if(!ioctl(sockfd, SIOCSIFMTU, &ifr)) {
    printf("MTU changed successfully.\n");
  }
}

// based on ifconfig source code
// void configure_ip_mode(char* iface, char* ip_addr){
//   struct ifreq ifr;
//   struct sockaddr_in sai;
//   int sockfd;
//   char *p;
//
//   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//
//   // setting interface name
//   strncpy(ifr.ifr_name, iface, IFNAMSIZ); // IFNAMSIZ is the maximum iface name length
//
//   memset(&sai, 0, sizeof(struct sockaddr));
//   sai.sin_family = AF_INET;
//   sai.sin_port = 0;
//
//   sai.sin_addr.s_addr = inet_addr(ip_addr);
//
//   p = (char *) &sai;
//   memcpy( (((char *)&ifr + ifreq_offsetof(ifr_addr) )), p, sizeof(struct sockaddr));
//
//   ioctl(sockfd, SIOCSIFADDR, &ifr);
//   ioctl(sockfd, SIOCGIFFLAGS, &ifr);
//
//   ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
//
//   ioctl(sockfd, SIOCSIFFLAGS, &ifr);
//   close(sockfd);
// }

void configure_ip_mode(char* iface, char* ip_addr){
    struct ifreq ifr;
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strncpy(ifr.ifr_name, iface, IFNAMSIZ); //IFNAMSIZ is the iface's max name length
    ifr.ifr_addr.sa_family = AF_INET;
    inet_pton(AF_INET, ip_addr, ifr.ifr_addr.sa_data + 2);
    ioctl(sockfd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, "255.255.0.0", ifr.ifr_addr.sa_data + 2);
    ioctl(sockfd, SIOCSIFNETMASK, &ifr);

    ioctl(sockfd, SIOCGIFFLAGS, &ifr);
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

    ioctl(sockfd, SIOCSIFFLAGS, &ifr);
}

int main(int argc, char** argv){
  unsigned int mode = decide_mode(argc, argv);
  switch(mode){
    case DEFAULT_MODE:
      printf("You chose default mode.\n"); //DEBUG
    break;

    case CONFIG_IP_MODE:
      printf("You chose ip configuration mode.\n"); // DEBUG
      configure_ip_mode(argv[1], argv[2]);
      printf("Interface's IP is configured as desired.\n"); // DEBUG
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
