#include "../include/arp_request.h"

int	create_raw_socket(void)
{
	int	sock;

	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0)
	{
		perror("Error creating socket (run as root)");
		return (-1);
	}
	return (sock);
}

static int	try_interface(int sock, struct ifreq *ifr, char *name)
{
	strncpy(ifr->ifr_name, name, IFNAMSIZ - 1);
	ifr->ifr_name[IFNAMSIZ - 1] = '\0';
	if (ioctl(sock, SIOCGIFINDEX, ifr) == 0)
		return (1);
	return (0);
}

static int	find_network_interface(int sock, t_network_info *net_info)
{
	struct ifreq	ifr;

	if (try_interface(sock, &ifr, "eth0") ||
		try_interface(sock, &ifr, "enp0s3") ||
		try_interface(sock, &ifr, "wlan0"))
	{
		net_info->ifindex = ifr.ifr_ifindex;
		strncpy(net_info->ifname, ifr.ifr_name, IFNAMSIZ - 1);
		net_info->ifname[IFNAMSIZ - 1] = '\0';
		return (1);
	}
	return (0);
}

static int	get_mac_and_ip(int sock, t_network_info *net_info)
{
	struct ifreq		ifr;
	struct sockaddr_in	*sin;

	strncpy(ifr.ifr_name, net_info->ifname, IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
		return (0);
	memcpy(net_info->src_mac, ifr.ifr_hwaddr.sa_data, MAC_ADDR_LEN);
	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
		return (0);
	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	net_info->src_ip = sin->sin_addr;
	return (1);
}

int	get_interface_info(int sock, t_network_info *net_info)
{
	if (!find_network_interface(sock, net_info))
	{
		printf("Error: No suitable network interface found\n");
		return (0);
	}
	printf("Using interface: %s\n", net_info->ifname);
	if (!get_mac_and_ip(sock, net_info))
	{
		perror("Error getting interface information");
		return (0);
	}
	printf("Using MAC address: ");
	print_mac_address(net_info->src_mac);
	printf("\nUsing IP address: %s\n", inet_ntoa(net_info->src_ip));
	return (1);
}

int	setup_network(t_network_info *net_info)
{
	net_info->sock = create_raw_socket();
	if (net_info->sock < 0)
		return (0);
	if (!get_interface_info(net_info->sock, net_info))
	{
		close(net_info->sock);
		return (0);
	}
	return (1);
}

void	cleanup_network(t_network_info *net_info)
{
	if (net_info->sock >= 0)
		close(net_info->sock);
}
