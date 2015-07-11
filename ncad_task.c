#include "ncad_task.h"
#include <stdio.h>
#include <stdlib.h>

/* ARPはRFC826 らしい */

struct ncad_task_context context;

int main (){

    if(!mac_table_init()) {
        fprintf(stderr, "mac_table_init error");
        exit(1);
    }
    server_start();

    return 0;
}
