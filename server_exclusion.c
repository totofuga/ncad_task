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

extern struct ncad_task_context context;

void server_exclusion(const struct ether_arp *arppack) {

    /* 送信先設定 */
    struct sockaddr_ll sockaddr;
    memset(&sockaddr, 0x0, sizeof(sockaddr));
    sockaddr.sll_family   = AF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ARP);
    sockaddr.sll_pkttype  = PACKET_HOST;
    sockaddr.sll_halen    = ETH_ALEN;

    sockaddr.sll_ifindex = if_nametoindex(context.interface_name);
    memcpy(sockaddr.sll_addr, arppack->arp_sha, 6);

    /* パケット作成 */
    struct  ether_arp replay_arppack;

    int arp_sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

    memset(&replay_arppack, 0x0, sizeof(replay_arppack));
    replay_arppack.arp_hrd = htons(1);
    replay_arppack.arp_pro = htons(ETHERTYPE_IP);
    replay_arppack.arp_hln = 6; /* MACアドレスの長さ */
    replay_arppack.arp_pln = 4; /* IPアドレスの長さ  */
    replay_arppack.arp_op  = htons(ARPOP_REQUEST);

    /* 送信先は不正な受信元*/
    memcpy(replay_arppack.arp_tha, arppack->arp_sha, 6);
    memcpy(replay_arppack.arp_tpa, arppack->arp_spa, 4);

    /* IPは探索に来たIPと見せかける */
    memcpy(replay_arppack.arp_spa, arppack->arp_tpa,4);

    /* そしてMACアドレスにゴミを入れる*/
    replay_arppack.arp_sha[0] = 0;
    replay_arppack.arp_sha[1] = 0;
    replay_arppack.arp_sha[2] = 0;
    replay_arppack.arp_sha[3] = 0;
    replay_arppack.arp_sha[4] = 0;
    replay_arppack.arp_sha[5] = 1;

    if(sendto(context.socket,(char *)&replay_arppack, sizeof(replay_arppack),
               0, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
          perror("exclusion sendto error");
          exit(1);
    }
}
