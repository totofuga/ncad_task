#include "ncad_task.h"

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

static int _create_socket(void);
static void _print_ip(char* name, unsigned char *ipaddr);
static void _print_ethaddr(char* name, unsigned char *ethaddr);

void server_start() {
    context.socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

    if(context.socket == -1 ) {
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
    sockaddr.sll_ifindex  = 0; /* 0だと任意のinterfaceにマッチ */
    sockaddr.sll_pktype   = PACKET_BROADCAST;
    sockaddr.sll_halen    = ETH_ALEN;

    if(bind(context.socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1){
        perror("bind error");
        exit(1);
    }

    while(1) {
        memset(buf, 0, sizeof(buf));
        arp_size = recv(context.socket, buf, sizeof(buf), 0); 

        if(arp_size < 0) {
            perror("arp read error");
            exit(1);
        }

        arppack = (struct ether_arp*) buf;

        /* arpのテーブル更新情報取得のみ取得するのでREQUESTでかつブロードキャストのみ取得*/
        if(ntohs(arppack->ea_hdr.ar_op) != ARPOP_REQUEST || !arp_is_target_broadcast(arppack))  {
            continue;
        }

        /* macアドレスが許可テーブルに登録されているかどうか調べる */
        if(!mac_table_has(arppack->arp_sha)) {
            switch(context.mode) {
                case RUN_MODE_DETECT:
                    server_detect(arppack);
                    break;
            }
            continue;
        }


        printf("operation : %d\n", ntohs(arppack->ea_hdr.ar_op));
        _print_ethaddr("sender hardware address", arppack->arp_sha);
        _print_ip("sender protocol address", arppack->arp_spa);
        _print_ethaddr("target hardware address", arppack->arp_tha);
        _print_ip("target protocol address", arppack->arp_tpa);
    }
}

void _print_ip(char* name, unsigned char *ipaddr) {
   printf("%s : %3d.%3d.%3d.%3d\n", name, ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
}
void _print_ethaddr(char* name, unsigned char *ethaddr) {
   printf("%s : %02x:%02x:%02x:%02x:%02x:%02x\n",name, ethaddr[0],ethaddr[1],ethaddr[2],ethaddr[3],ethaddr[4],ethaddr[5]);
}
