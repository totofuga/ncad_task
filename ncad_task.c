#include "ncad_task.h"
#include <stdio.h>
#include <stdlib.h>

/* ARP(RFC826 らしい) */

struct ncad_task_context context;

int main (int argc, char *argv[]){

    context.mode       = RUN_MODE_DETECT;
    context.email_from = "fujita.yoshihiko+from@gmail.com";
    context.email_to   = "fujita.yoshihiko+to@gmail.com";

    if(!mac_table_init()) {
        fprintf(stderr, "mac_table_init error");
        exit(1);
    }
    server_start();

    return 0;
}
