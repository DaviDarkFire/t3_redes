#include "xarpd.h"

int global_ttl = 60;

struct iface my_ifaces[MAX_IFACES];
//
// Print an Ethernet address
void print_eth_address(char *s, unsigned char *eth_addr){
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
	struct iface *ifn = (struct iface*) arg;

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
		ifn->rx_pkts++;
		ifn->rx_bytes += n;
		doProcess(packet_buffer, n);
		// free(arg);
	}
}

void daemon_handle_request(unsigned char* request, int sockfd, node_t** head, unsigned int qt_ifaces){
	int opcode = request[0] - '0';
	FILE * fp = fdopen(sockfd, "w");

	switch(opcode){
		case XARP_SHOW:
			print_list(*head, fp);
			break;

		case XARP_RES:
			break;

		case XARP_ADD:{
			unsigned int ip_address = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
			unsigned char eth_address[6];
			memcpy(eth_address, request+1+4, 6); // 1B for opcode, 4B for ip address, 6B for eth_address
			int ttl = (request[14] << 24) | (request[13] << 16) | (request[12] << 8) | (request[11]);
			node_t* found_node = find_node_by_ip_address(*head, ip_address);

			if(found_node == NULL){
				fprintf(fp, "Node not found, adding new node\n"); // DEBUG
				if(ttl == -1)
					add_node(head, ip_address, eth_address, global_ttl);
				else
					add_node(head, ip_address, eth_address, ttl);
				// print_list(*head, fp); // DEBUG
			} else {
				fprintf(fp, "Node found, modifying node\n"); // DEBUG
				found_node->ip_address = ip_address;
				memcpy(found_node->eth_address, eth_address, 6);
				found_node->ttl = (ttl == -1 ? global_ttl : ttl);
			}
			fprintf(fp, "Successfull add.\n"); // DEBUG
			break;
		}

		case XARP_DEL:{
			unsigned int ip_address = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
			if(delete_node_by_ip_address(head, ip_address) == 1)
			  fprintf(fp, "Node deleted succesfully.\n"); // DEBUG
			else
				fprintf(fp, "Couldn't delete node.\n"); // DEBUG
			break;
		}

		case XARP_TTL:{
			int ttl = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
			global_ttl = ttl;
			fprintf(fp, "New default TTL is: %d\n", global_ttl);
			break;
		}

		case XIFCONFIG_INFO:{
			unsigned int i;
			for(i = 0; i < qt_ifaces; i++){
				print_iface_info(i);
			}
			break;
		}

		case XIFCONFIG_IP:{
			break;
		}

		case XIFCONFIG_MTU:{
			break;
		}

		default:
			fprintf(fp, "Daemon couldn't recognize this request.\n"); // DEBUG
	}
	fclose(fp);
}

/* */
// main function
int main(int argc, char** argv) {
	int	i, sockfd;

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

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&(tid[i]), &attr, read_iface, &my_ifaces[i]);
		// print_iface_info(i); // DEBUG
		// Create one thread for each interface. Each thread should run the function read_iface.
	}

	node_t* head = NULL;

	int listen_sockfd;
	int clilen;
	int connfd;
	unsigned char buffer[BUFFSIZE];
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sockfd < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	memset((char*) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if(bind(listen_sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	if(listen(listen_sockfd, LISTEN_ENQ) < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}

	clilen = sizeof(cli_addr);

	while(1) {
		connfd = accept(listen_sockfd, (struct sockaddr*) &cli_addr, (unsigned int*) &clilen);
		if(connfd < 0) {
			printf("Erro no if do accept\n"); // DEBUG
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}

		memset(buffer, 0, sizeof(buffer));

		if(recv(connfd, buffer, sizeof(buffer), 0) < 0) {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}

		printf("Received message: %s\n", buffer);

		daemon_handle_request(buffer, connfd, &head, argc-1);
	}

	for(i = 0; i < argc-1; i++){
		pthread_join(tid[i], NULL);
	}

	return 0;
}
/* */
