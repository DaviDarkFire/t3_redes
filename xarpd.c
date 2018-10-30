#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
/* */
/* */
#define MAX_PACKET_SIZE 65536
#define MIN_PACKET_SIZE 64
/* */
#define MAX_IFACES	64
#define MAX_IFNAME_LEN	22
#define ETH_ADDR_LEN	6
/* */
struct iface {
	int		sockfd;
	int		ttl;
	int		mtu;
	char		ifname[MAX_IFNAME_LEN];
	unsigned char	mac_addr[6];
	unsigned int	ip_addr;
	unsigned int	rx_pkts;
	unsigned int	rx_bytes;
	unsigned int	tx_pkts;
	unsigned int	tx_bytes;
};
/* */
struct ether_hdr {
	unsigned char	ether_dhost[6];	// Destination address
	unsigned char	ether_shost[6];	// Source address
	unsigned short	ether_type;	// Type of the payload
};
/* */
struct ip_hdr {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	unsigned char	ip_ihl:4,
								ip_v:4;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	unsigned char	ip_v:4,
								ip_ihl:4;

#endif
	unsigned char	ip_tos;		// Type of service
	unsigned short	ip_len;		// Datagram Length
	unsigned short	ip_id;		// Datagram identifier
	unsigned short	ip_offset;	// Fragment offset
	unsigned char	ip_ttl;		// Time To Live
	unsigned char	ip_proto;	// Protocol
	unsigned short	ip_csum;	// Header checksum
	unsigned int	ip_src;		// Source IP address
	unsigned int	ip_dst;		// Destination IP address
};
/* */
// Read RFC 826 to define the ARP struct
struct arp_hdr{
	unsigned short hw_type;
	unsigned short proto_type;
	unsigned char hw_addr_len;
	unsigned char proto_addr_len;
	unsigned short opcode;
	unsigned int sender_ip_addr;
	unsigned char sender_hw_addr[6];
	unsigned int sender_proto_addr;
	unsigned char target_hw_addr;
	unsigned int target_proto_addr;
};
/* */
//
//
struct iface	my_ifaces[MAX_IFACES];
//
// Print an Ethernet address
void print_eth_address(char *s, unsigned char *eth_addr)
{
	printf("%s %02X:%02X:%02X:%02X:%02X:%02X", s,
	       eth_addr[0], eth_addr[1], eth_addr[2],
	       eth_addr[3], eth_addr[4], eth_addr[5]);
}

void print_iface_info(unsigned int iface_index){
	struct sockaddr_in sa;
	sa.sin_addr.s_addr = (unsigned long) my_ifaces[iface_index].ip_addr;

	printf("sockfd: %d\n",my_ifaces[iface_index].sockfd);
	printf("ttl: %d\n", my_ifaces[iface_index].ttl);
	printf("mtu: %d\n", my_ifaces[iface_index].mtu);
	printf("ifname: %s\n", my_ifaces[iface_index].ifname);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
	my_ifaces[iface_index].mac_addr[0],
 	my_ifaces[iface_index].mac_addr[1],
	my_ifaces[iface_index].mac_addr[2],
	my_ifaces[iface_index].mac_addr[3],
	my_ifaces[iface_index].mac_addr[4],
	my_ifaces[iface_index].mac_addr[5]);
	printf("IP: %s\n", inet_ntoa(sa.sin_addr));
	printf("RX packets: %d\n", my_ifaces[iface_index].rx_pkts);
	printf("RX bytes: %d\n", my_ifaces[iface_index].rx_bytes);
	printf("TX packets: %d\n", my_ifaces[iface_index].tx_pkts);
	printf("TX bytes: %d\n", my_ifaces[iface_index].tx_bytes);

}
/* */
// Bind a socket to an interface
int bind_iface_name(int fd, char *iface_name)
{
	return setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, iface_name, strlen(iface_name));
}
/* */
void get_iface_info(int sockfd, char *ifname, struct iface *ifn)
{
	struct ifreq s;

	strcpy(s.ifr_name, ifname);
	if (0 == ioctl(sockfd, SIOCGIFHWADDR, &s)) {
		memcpy(ifn->mac_addr, s.ifr_addr.sa_data, ETH_ADDR_LEN);
		ifn->sockfd = sockfd;
		strcpy(ifn->ifname, ifname);
	} else {
		perror("Error getting MAC address");
		exit(1);
	}
}
// Print the expected command line for the program
void print_usage()
{
	printf("\xarpd <interface> [<interfaces>]\n");
	exit(1);
}
/* */
// Break this function to implement the ARP functionalities.
void doProcess(unsigned char* packet, int len) {
	if(!len || len < MIN_PACKET_SIZE)
		return;

	struct ether_hdr* eth = (struct ether_hdr*) packet;

	if(htons(0x0806) == eth->ether_type) { // ARP

	}
	// Ignore if it is not an ARP packet
}
/* */
// This function should be one thread for each interface.
void* read_iface(void *arg)
{
	struct iface *ifn = *((struct iface**) arg);

	socklen_t	saddr_len;
	struct sockaddr	saddr;
	unsigned char	*packet_buffer;
	int		n;

	saddr_len = sizeof(saddr);
	packet_buffer = malloc(MAX_PACKET_SIZE);
	if (!packet_buffer) {
		printf("\nCould not allocate a packet buffer\n");
		exit(1);
	}

	while(1) {
		n = recvfrom(ifn->sockfd, packet_buffer, MAX_PACKET_SIZE, 0, &saddr, &saddr_len);
		if(n < 0) {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}
		doProcess(packet_buffer, n);
		free(arg);
	}
}
/* */
// main function
int main(int argc, char** argv) {
	int		i, sockfd;

	if (argc < 2)
		print_usage();

	for (i = 1; i < argc; i++) {
		sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(sockfd < 0) {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}

		if (bind_iface_name(sockfd, argv[i]) < 0) {
			perror("Server-setsockopt() error for SO_BINDTODEVICE");
			printf("%s\n", strerror(errno));
			close(sockfd);
			exit(1);
		}
		get_iface_info(sockfd, argv[i], &my_ifaces[i-1]);
	}

	pthread_t tid[argc-1];
	for (i = 0; i < argc-1; i++) {
		struct iface *arg = malloc(sizeof(*arg));
		print_eth_address(my_ifaces[i].ifname, my_ifaces[i].mac_addr);
		printf("\n");
		*arg = my_ifaces[i];

		pthread_create(&(tid[i]), NULL, read_iface, arg);
		print_iface_info(i);
		// Create one thread for each interface. Each thread should run the function read_iface.
	}

	for(i = 0; i < argc-1; i++){
		pthread_join(tid[i], NULL);
	}

	//TODO: colocar aqui a execução da thread principal que fica escutando request dos programas auxiliares

	return 0;
}
/* */
