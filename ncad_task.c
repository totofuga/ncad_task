#include "ncad_task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <getopt.h>

/* ARP(RFC826 らしい) */

struct ncad_task_context context;

void _register_my_macaddr();

int main (int argc, char *argv[]) {

    int opt;

    // default
    context.mode            = RUN_MODE_DETECT;
    context.email_from      = "fujita.yoshihiko+from@gmail.com";
    context.email_to        = "fujita.yoshihiko+to@gmail.com";
    context.interface_name  = "eth0";

    while ((opt = getopt(argc, argv, "det:")) != -1) {
        switch (opt) {
            case 'd':
                context.mode = RUN_MODE_DETECT;
                break;
            case 'e':
                context.mode = RUN_MODE_EXCLUSION;
                break;
            case 't':
                context.mode = RUN_MODE_EXCLUSION;
                context.delay_sec = atol(optarg);
                break;
            case 'i':
                break;
        }
    }

    context.socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    if(context.socket == -1 ) {
        perror("socket error");
        exit(1);
    }

    if(!mac_table_init()) {
        fprintf(stderr, "mac_table_init error");
        exit(1);
    }

    /* 自分自身のMACアドレスは拒否しないように最初に登録する */
    _register_my_macaddr();

    server_start();

    return 0;
}

void _register_my_macaddr() {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, context.interface_name, strlen(context.interface_name));
    if(ioctl(context.socket, SIOCGIFHWADDR, &ifr)) {
        perror("_register_my_macaddr error");
        exit(1);
    }

    mac_table_add(ifr.ifr_hwaddr.sa_data);
}
