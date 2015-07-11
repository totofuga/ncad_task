#include <netinet/if_ether.h>

typedef enum {
    RUN_MODE_DETECT,    /* 検知 */
    RUN_MODE_EXCLUSION, /* 排除 */
    RUN_MODE_DELAY      /* 遅延 */
} run_mode;

struct ncad_task_context {
    int socket;
    run_mode mode;
    long delay_sec;
    char* email_from;
    char* email_to;
    char* interface_name;
}; 

extern struct ncad_task_context context;

int arp_is_target_broadcast (const struct ether_arp* arppack);
void server_start();

/* macアドレス許可テーブル */
int mac_table_init();
void mac_table_add(const unsigned char* macaddr);
int mac_table_has(const unsigned char *macaddr);

/* 検知*/
void server_detect(const struct ether_arp *arppack);

/* 排除 */
void server_exclusion(const struct ether_arp *arppack);
