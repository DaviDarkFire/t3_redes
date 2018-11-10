#ifndef __XARPD__
#define __XARPD__
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <ifaddrs.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <net/ethernet.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <pthread.h>
	#include "linked_list.h"
	#include "communication.h"
	#include <linux/if.h>
	#include "arp_protocol.h"
	/* */
	/* */
	#define MAX_PACKET_SIZE 65536
	#define MIN_PACKET_SIZE 64
	/* */
	#define MAX_IFACES	64
	#define MAX_IFNAME_LEN	6
	#define ETH_ADDR_LEN	6

	#define BUFFSIZE 1024
	#define LISTEN_ENQ 5

/* */
struct iface {
	int		sockfd;
	int		ttl;
	int		mtu;
	char		ifname[MAX_IFNAME_LEN];
	unsigned char	mac_addr[6];
	// unsigned int	ip_addr;
	// unsigned int netmask;
	// unsigned int bcast_addr;
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
// struct arp_hdr{
// 	unsigned short hw_type;
// 	unsigned short proto_type;
// 	unsigned char hw_addr_len;
// 	unsigned char proto_addr_len;
// 	unsigned short opcode;
// 	unsigned char sender_hw_addr[6];
// 	unsigned int sender_proto_addr;
// 	unsigned char target_hw_addr[6];
// 	unsigned int target_proto_addr;
// };
/* */
//
//

void print_eth_address(char *s, unsigned char *eth_addr);
void print_iface_info(int sockfd, FILE* fp, unsigned int iface_index);
int bind_iface_name(int fd, char *iface_name);
void get_iface_info(int sockfd, char *ifname, struct iface *ifn);
void print_usage();
void doProcess(unsigned char* packet, int len);
void* read_iface(void *arg);
void daemon_handle_request(unsigned char* request, int sockfd, node_t** head, unsigned int qt_ifaces);
char* get_ip_address_as_dotted_dec(char* ifname);
char* get_bcast_address_as_dotted_dec(char* ifname);
char* get_netmask_as_dotted_dec(char* ifname);
unsigned int get_iface_index(char* iface_name);
void update_mtu(char* ifname);
#endif
