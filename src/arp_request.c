#include "../include/arp_request.h"

int	main(int argc, char **argv)
{
	t_network_info	net_info;
	struct in_addr	dst_ip;

	if (argc != 2)
		print_usage(argv[0]);
	if (!validate_ip(argv[1], &dst_ip))
		exit(1);
	printf("Sending ARP request: Who has %s?\n", argv[1]);
	if (!setup_network(&net_info))
		exit(1);
	printf("Sending ARP request: Who has %s? Tell %s\n",
		argv[1], inet_ntoa(net_info.src_ip));
	if (!send_arp_packet(&net_info, &dst_ip))
	{
		cleanup_network(&net_info);
		exit(1);
	}
	print_packet_info(argv[1], inet_ntoa(net_info.src_ip));
	cleanup_network(&net_info);
	return (0);
}
