#ifndef ARP_REQUEST_H
# define ARP_REQUEST_H

# define _GNU_SOURCE
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <net/ethernet.h>
# include <netpacket/packet.h>
# include <net/if.h>
# include <sys/ioctl.h>
# include <net/if_arp.h>
# include <netinet/in.h>
# include <linux/if.h>

# define ARP_PACKET_SIZE 42
# define ETH_HEADER_SIZE 14
# define ARP_HEADER_SIZE 28
# define MAC_ADDR_LEN 6
# define IP_ADDR_LEN 4

typedef struct s_eth_header
{
	unsigned char	dest[MAC_ADDR_LEN];
	unsigned char	src[MAC_ADDR_LEN];
	unsigned short	ethertype;
}	__attribute__((packed)) t_eth_header;

typedef struct s_arp_header
{
	unsigned short	htype;
	unsigned short	ptype;
	unsigned char	hlen;
	unsigned char	plen;
	unsigned short	opcode;
	unsigned char	sender_mac[MAC_ADDR_LEN];
	unsigned char	sender_ip[IP_ADDR_LEN];
	unsigned char	target_mac[MAC_ADDR_LEN];
	unsigned char	target_ip[IP_ADDR_LEN];
}	__attribute__((packed)) t_arp_header;

typedef struct s_network_info
{
	int				sock;
	int				ifindex;
	char			ifname[IFNAMSIZ];
	unsigned char	src_mac[MAC_ADDR_LEN];
	struct in_addr	src_ip;
}	t_network_info;

/* utils.c */
void			print_usage(char *program);
int				validate_ip(char *ip_str, struct in_addr *ip_addr);
void			print_mac_address(unsigned char *mac);
void			print_packet_info(char *target_ip, char *source_ip);

/* network.c */
int				create_raw_socket(void);
int				get_interface_info(int sock, t_network_info *net_info);
int				setup_network(t_network_info *net_info);
void			cleanup_network(t_network_info *net_info);

/* packet.c */
void			build_ethernet_header(t_eth_header *eth,
					unsigned char *src_mac);
void			build_arp_header(t_arp_header *arp, unsigned char *src_mac,
					struct in_addr *src_ip, struct in_addr *dst_ip);
int				send_arp_packet(t_network_info *net_info,
					struct in_addr *dst_ip);

#endif
