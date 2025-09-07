#include "../include/arp_request.h"

void	print_usage(char *program)
{
	printf("Usage: %s <target_ip>\n", program);
	printf("Example: %s 192.168.1.1\n", program);
	printf("\nSends an ARP request asking: 'Who has target_ip?'\n");
	printf("Uses the local machine's IP and MAC automatically.\n");
	exit(1);
}

int	validate_ip(char *ip_str, struct in_addr *ip_addr)
{
	if (inet_aton(ip_str, ip_addr) == 0)
	{
		printf("Error: Invalid IP address: %s\n", ip_str);
		return (0);
	}
	return (1);
}

void	print_mac_address(unsigned char *mac)
{
	printf("%02x:%02x:%02x:%02x:%02x:%02x",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void	print_packet_info(char *target_ip, char *source_ip)
{
	printf("ARP request sent successfully!\n");
	printf("Packet details:\n");
	printf("  - Ethernet frame: %d bytes\n", ETH_HEADER_SIZE);
	printf("  - ARP payload: %d bytes\n", ARP_HEADER_SIZE);
	printf("  - Total: %d bytes\n", ARP_PACKET_SIZE);
	printf("  - Question: Who has %s? Tell %s\n", target_ip, source_ip);
}
