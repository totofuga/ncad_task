/*
 不正な接続先が大量に増える事は想定しにくいので
 IOとsleepを多重化する為にプロセスをフォークしてしまう。
*/

#include "ncad_task.h"
#include <stdlib.h>
#include <stdio.h>

extern struct ncad_task_context context;

void server_delay(const struct ether_arp *arppack) {
    switch(fork()) {
        case -1:
            perror("server_delay fork error");
            exit(1);
            break;
        case 0: /* child */
            sleep(context.delay_sec);
            server_exclusion(arppack);
            exit(0);
            break;
        default: /* parent */
            break;
    }
}
