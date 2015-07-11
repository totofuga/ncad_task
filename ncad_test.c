#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/if.h>

#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>

static int create_socket(void);
static void print_ip(char* name, unsigned char *ipaddr);
static void print_ethaddr(char* name, unsigned char *ethaddr);

//ARPはRFC826 らしい

int main (){
    int arp_sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

    if(arp_sock == -1 ) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_ll sockaddr;
    char buf[256];
    int arp_size;
    struct ether_arp *arppack;

    memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sll_family   = AF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ARP);
    sockaddr.sll_ifindex  = 0; // 0だと任意のinterfaceにマッチ
    sockaddr.sll_ifindex  = PACKET_HOST; // TODO:ブロードキャストだけ取得だとうまくいかなかったあとで原因しらべる

    if(bind(arp_sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1){
        perror("bind error");
        exit(1);
    }

    while(1) {
        memset(buf, 0, sizeof(buf));
        arp_size = recv(arp_sock, buf, sizeof(buf), 0); 

        if(arp_size < 0) {
            perror("arp read error");
            exit(1);
        }

        arppack = (struct ether_arp*) buf;

        printf("operation : %d\n", ntohs(arppack->ea_hdr.ar_op));
        print_ethaddr("sender hardware address", arppack->arp_sha);
        print_ip("sender protocol address", arppack->arp_spa);
        print_ethaddr("target hardware address", arppack->arp_tha);
        print_ip("target protocol address", arppack->arp_tpa);
    }

    return 0;
}

void print_ip(char* name, unsigned char *ipaddr) {
   printf("%s : %3d.%3d.%3d.%3d\n", name, ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
}
void print_ethaddr(char* name, unsigned char *ethaddr) {
   printf("%s : %02x:%02x:%02x:%02x:%02x:%02x\n",name, ethaddr[0],ethaddr[1],ethaddr[2],ethaddr[3],ethaddr[4],ethaddr[5]);
}



