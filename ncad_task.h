#include <netinet/if_ether.h>

typedef enum {
    RUN_MODE_DETECT,    /* 検知 */
    RUN_MODE_EXCLUSION, /* 排除 */
    RUN_MODE_DELAY      /* 遅延 */
} run_mode; 

struct ncad_task_context {
    run_mode mode;
    long delay_sec;
}; 

extern struct ncad_task_context context;

int arp_is_target_broadcast (const struct ether_arp* arppack);
void server_start();

/* macアドレス許可テーブル */
int mac_table_init();
void mac_table_add(char* macaddr);
int mac_table_has(char *macaddr);
