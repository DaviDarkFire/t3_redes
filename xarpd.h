#ifndef __XARPD__
#define __XARPD__
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
	#include "linked_list.h"
	#include "communication.h"
	#include <linux/if.h>
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

void print_eth_address(char *s, unsigned char *eth_addr);
void print_iface_info(unsigned int iface_index);
int bind_iface_name(int fd, char *iface_name);
void get_iface_info(int sockfd, char *ifname, struct iface *ifn);
void print_usage();
void doProcess(unsigned char* packet, int len);
void* read_iface(void *arg);
void arp_handle_request(char* received_buffer, node_t* list_head, int connfd);
#endif
