#include "../include/arp_request.h"

void	build_ethernet_header(t_eth_header *eth, unsigned char *src_mac)
{
	unsigned char	broadcast_mac[MAC_ADDR_LEN];
	int				i;

	i = 0;
	while (i < MAC_ADDR_LEN)
	{
		broadcast_mac[i] = 0xff;
		i++;
	}
	memcpy(eth->dest, broadcast_mac, MAC_ADDR_LEN);
	memcpy(eth->src, src_mac, MAC_ADDR_LEN);
	eth->ethertype = htons(0x0806);
}

void	build_arp_header(t_arp_header *arp, unsigned char *src_mac,
		struct in_addr *src_ip, struct in_addr *dst_ip)
{
	arp->htype = htons(1);
	arp->ptype = htons(0x0800);
	arp->hlen = MAC_ADDR_LEN;
	arp->plen = IP_ADDR_LEN;
	arp->opcode = htons(1);
	memcpy(arp->sender_mac, src_mac, MAC_ADDR_LEN);
	memcpy(arp->sender_ip, &src_ip->s_addr, IP_ADDR_LEN);
	memset(arp->target_mac, 0, MAC_ADDR_LEN);
	memcpy(arp->target_ip, &dst_ip->s_addr, IP_ADDR_LEN);
}

static int	setup_sockaddr(struct sockaddr_ll *addr, t_network_info *net_info)
{
	unsigned char	broadcast_mac[MAC_ADDR_LEN];
	int				i;

	i = 0;
	while (i < MAC_ADDR_LEN)
	{
		broadcast_mac[i] = 0xff;
		i++;
	}
	addr->sll_family = AF_PACKET;
	addr->sll_protocol = htons(ETH_P_ARP);
	addr->sll_ifindex = net_info->ifindex;
	addr->sll_hatype = ARPHRD_ETHER;
	addr->sll_pkttype = PACKET_BROADCAST;
	addr->sll_halen = MAC_ADDR_LEN;
	memcpy(addr->sll_addr, broadcast_mac, MAC_ADDR_LEN);
	return (1);
}

int	send_arp_packet(t_network_info *net_info, struct in_addr *dst_ip)
{
	unsigned char		packet[ARP_PACKET_SIZE];
	struct sockaddr_ll	addr;
	t_eth_header		*eth;
	t_arp_header		*arp;

	memset(packet, 0, ARP_PACKET_SIZE);
	eth = (t_eth_header *)packet;
	arp = (t_arp_header *)(packet + ETH_HEADER_SIZE);
	build_ethernet_header(eth, net_info->src_mac);
	build_arp_header(arp, net_info->src_mac, &net_info->src_ip, dst_ip);
	memset(&addr, 0, sizeof(addr));
	setup_sockaddr(&addr, net_info);
	if (sendto(net_info->sock, packet, ARP_PACKET_SIZE, 0,
			(struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Error sending ARP request");
		return (0);
	}
	return (1);
}
