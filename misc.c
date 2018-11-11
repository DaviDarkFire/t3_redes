#include "misc.h"

unsigned char* get_ip_addr_bytes_from_string(char* dotted_dec_ip){
  int values[4];
  int i;
  unsigned char* ip_bytes;
  ip_bytes = malloc(sizeof(char)*4);

  sscanf(dotted_dec_ip, "%d.%d.%d.%d",
  &values[3], &values[2], &values[1], &values[0]);

  for(i = 0; i < 4; i++){
    ip_bytes[i] = (unsigned char) values[i];
  }
  return ip_bytes;
}

char* get_ip_address_as_dotted_dec(char* ifname){
	int fd;
 	struct ifreq ifr;
	char * ip_address = malloc(sizeof(char)*(16));
 	fd = socket(AF_INET, SOCK_DGRAM, 0);

 	ifr.ifr_addr.sa_family = AF_INET;
 	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
 	ioctl(fd, SIOCGIFADDR, &ifr);

 	close(fd);

	ip_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

 	return ip_address;
}

char* get_bcast_address_as_dotted_dec(char* ifname){
	int fd;
 	struct ifreq ifr;
	char * ip_address = malloc(sizeof(char)*(15));
 	fd = socket(AF_INET, SOCK_DGRAM, 0);

 	ifr.ifr_addr.sa_family = AF_INET;
 	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
 	ioctl(fd, SIOCGIFBRDADDR, &ifr);

 	close(fd);

	ip_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr);

 	return ip_address;
}

char* get_netmask_as_dotted_dec(char* ifname){
	int fd;
 	struct ifreq ifr;
	char * ip_address = malloc(sizeof(char)*(15));
 	fd = socket(AF_INET, SOCK_DGRAM, 0);

 	ifr.ifr_addr.sa_family = AF_INET;
 	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
 	ioctl(fd, SIOCGIFNETMASK, &ifr);

 	close(fd);

	ip_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr);

 	return ip_address;
}

void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

// int main(void){

// 	return 0;
// }
