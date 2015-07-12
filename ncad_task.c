#include "ncad_task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <getopt.h>
#include <signal.h>
#include <error.h>
#include <errno.h>

#define DEFAULT_CONFIG_PATH "./macaddr.conf"

/* ARP(RFC826 らしい) */

struct ncad_task_context context;

static void _register_my_macaddr();
static void _register_config_macaddr(const char* file_path);

int main (int argc, char *argv[]) {

    int opt;

    /* default value */
    char *macaddr_config_path = DEFAULT_CONFIG_PATH;
    context.mode              = RUN_MODE_DETECT;
    context.email_from        = "fujita.yoshihiko+from@gmail.com";
    context.email_to          = "fujita.yoshihiko+to@gmail.com";
    context.interface_name    = "eth0";
    context.debug = 0;

    while ((opt = getopt(argc, argv, "mes:i:f:t:dc:")) != -1) {
        switch (opt) {
            case 'm':
                context.mode = RUN_MODE_DETECT;
                break;
            case 'e':
                context.mode = RUN_MODE_EXCLUSION;
                break;
            case 's':
                context.mode = RUN_MODE_DELAY;
                context.delay_sec = atol(optarg);
                break;
            case 'i':
                context.interface_name  = optarg;
                break;
            case 'f':
                context.email_from = optarg;
                break;
            case 't':
                context.email_from = optarg;
                break;
            case 'd':
                context.debug = 1;
                break;
            case 'c':
                macaddr_config_path = optarg;
                break;
        }
    }

    //context.socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    //if(context.socket == -1 ) {
    //    perror("socket error");
    //    exit(1);
    //}

    /* 
        簡易的にRUN_MODE_DELAYの場合のゾンビ回避 
        TODO: 同じMacAddressに対して別のプロセスが複製されるのはが嫌な場合は
        ちゃんとsignalとってステータス管理する
    */
    signal(SIGCHLD, SIG_IGN);

    /* アクセス許可テーブルの初期化 */
    if(!mac_table_init()) {
        fprintf(stderr, "mac_table_init error");
        exit(1);
    }

    /* Configに書かれているMACアドレスを登録する */
    _register_config_macaddr(macaddr_config_path);

    /* 自分自身のMACアドレスは拒否しないように登録する */
    //_register_my_macaddr();

    //server_start();

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

void _register_config_macaddr(const char* file_path) {
    FILE *fp;
    char buf[255];
    char *s, *e, *p1, *p2; // 本来17文字だけど空白とか間違ったりを考慮して255文字取っておく
    unsigned char macaddr[6];
    int i, n;

    errno = 0;
    if ((fp = fopen(file_path, "r")) == NULL) {
        if(errno == ENOENT) {
            // デフォルトのままだと動かないので詳しいエラーを出しておく
            fprintf(stderr, "MACアドレスを記載したファイル[%s]が開けません。\nファイルを作成するかcオプションで正しいパスを指定してください。\n", file_path);
            exit(1);
        } else {
            perror("config open error");
            exit(1);
        }
    }

    while ( fgets(buf, 255, fp) != NULL ) {
        s = buf;
        while( isspace(*s) ){
            s++;
        }

        e = s;

        while (*e != '\0' && !isspace(*e)) {
            e++;
        }

        if (s == e) continue;

        *e = '\0';

        p1 = s;
        for (i = 0; i < 6; ++i) {
            n = strtol(p1, &p2, 16);
            if ( p1 == p2 ) {
                fprintf(stderr, "macアドレスの指定が不正です。(%s)\nxx:xx:xx:xx:xx:xxの形式で指定してください。\n[file: %s]\n", s, file_path);
                exit(1);
            }
            macaddr[i] = n;
            p1 = p2+1; // :飛ばし
        }

        mac_table_add(macaddr);
    }
}

