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

void server_start() {

    struct sockaddr_ll sockaddr;
    char buf[256];
    int arp_size;
    struct ether_arp *arppack;

    memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sll_family   = AF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ARP);
    sockaddr.sll_ifindex  = 0; /* 0だと任意のinterfaceにマッチ */
    sockaddr.sll_pkttype  = PACKET_BROADCAST;
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
            printf("MACADDR:[%02x:%02x:%02x:%02x:%02x:%02x]\n",
                    arppack->arp_sha[0],
                    arppack->arp_sha[1],
                    arppack->arp_sha[2],
                    arppack->arp_sha[3],
                    arppack->arp_sha[4],
                    arppack->arp_sha[5]);

            switch(context.mode) {
                case RUN_MODE_DETECT:
                    printf("DETECT\n");
                    server_detect(arppack);
                    break;
                case RUN_MODE_EXCLUSION:
                    printf("EXCLUSION\n");
                    server_exclusion(arppack);
                    break;
                case RUN_MODE_DELAY:
                    printf("DELAY\n");
                    server_delay(arppack);
                    break;
            }
            putchar('\n');
            continue;
        }


    }
}
