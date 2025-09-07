/* #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>

struct eth_header {
    unsigned char dest[6];
    unsigned char src[6];
    unsigned short ethertype;
} __attribute__((packed));

struct arp_header {
    unsigned short htype;
    unsigned short ptype;
    unsigned char hlen;
    unsigned char plen;
    unsigned short opcode;
    unsigned char sender_mac[6];
    unsigned char sender_ip[4];
    unsigned char target_mac[6];
    unsigned char target_ip[4];
} __attribute__((packed));

void print_usage(char *program)
{
    printf("Usage: %s <source_ip> <target_ip>\n", program);
    printf("Example: %s 192.168.1.10 192.168.1.1\n", program);
    printf("\nSends an ARP request asking: 'Who has target_ip? Tell source_ip'\n");
    exit(1);
}

int main(int ac, char **av) 
{
    int sock;
    unsigned char packet[42] = {0}; // 14 bytes Ethernet + 28 bytes ARP
    struct sockaddr_ll addr = {0};
    struct ifreq ifr;
    struct in_addr src_ip, dst_ip;

    // Validar argumentos
    if (ac != 3) {
        print_usage(av[0]);
    }

    // Validar IPs
    if (inet_aton(av[1], &src_ip) == 0) {
        printf("Error: Invalid source IP: %s\n", av[1]);
        exit(1);
    }
    if (inet_aton(av[2], &dst_ip) == 0) {
        printf("Error: Invalid target IP: %s\n", av[2]);
        exit(1);
    }

    printf("Sending ARP request: Who has %s? Tell %s\n", av[2], av[1]);

    // Abrir raw socket
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("Error creating socket (run as root)");
        exit(1);
    }

    // Obtener interfaz por defecto (intentar varias comunes)
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ-1);
        if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
            strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
            if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
                perror("Error getting interface index");
                close(sock);
                exit(1);
            }
        }
    }
    
    printf("Using interface: %s\n", ifr.ifr_name);
    int ifindex = ifr.ifr_ifindex;

    // Obtener MAC propia
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("Error getting MAC address");
        close(sock);
        exit(1);
    }
    unsigned char src_mac[6];
    memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);

    printf("Using MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
        src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);

    // Dirección destino (broadcast para ARP request)
    unsigned char dest_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

    // Construir cabecera Ethernet
    struct eth_header *eth = (struct eth_header *)packet;
    memcpy(eth->dest, dest_mac, 6);
    memcpy(eth->src, src_mac, 6);
    eth->ethertype = htons(0x0806); // ARP

    // Construir cabecera ARP
    struct arp_header *arp = (struct arp_header *)(packet + 14);
    arp->htype = htons(1);          // Ethernet
    arp->ptype = htons(0x0800);     // IPv4
    arp->hlen = 6;                  // MAC length
    arp->plen = 4;                  // IP length
    arp->opcode = htons(1);         // ARP Request
    
    // Sender (nosotros)
    memcpy(arp->sender_mac, src_mac, 6);
    memcpy(arp->sender_ip, &src_ip.s_addr, 4);
    
    // Target (quien buscamos)
    memset(arp->target_mac, 0, 6);   // Desconocido (por eso preguntamos)
    memcpy(arp->target_ip, &dst_ip.s_addr, 4);

    // Configurar dirección de destino
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ARP);
    addr.sll_ifindex = ifindex;
    addr.sll_hatype = ARPHRD_ETHER;
    addr.sll_pkttype = PACKET_BROADCAST;
    addr.sll_halen = 6;
    memcpy(addr.sll_addr, dest_mac, 6);

    // Enviar paquete
    if (sendto(sock, packet, 42, 0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error sending ARP request");
        close(sock);
        exit(1);
    }

    printf("ARP request sent successfully!\n");
    printf("Packet details:\n");
    printf("  - Ethernet frame: 14 bytes\n");
    printf("  - ARP payload: 28 bytes\n");
    printf("  - Total: 42 bytes\n");
    printf("  - Destination: Broadcast (ff:ff:ff:ff:ff:ff)\n");
    printf("  - Question: Who has %s? Tell %s\n", av[2], av[1]);
    
    close(sock);
    return 0;
}

 */