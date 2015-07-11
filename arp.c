#include <netinet/if_ether.h>
#include "ncad_task.h"

/* TODO:定数か関数ありそうあれば置き換える */
int arp_is_target_broadcast (const struct ether_arp* arppack)  {
    return 
        arppack->arp_tha[0] == 0x0 &&
        arppack->arp_tha[1] == 0x0 &&
        arppack->arp_tha[2] == 0x0 &&
        arppack->arp_tha[3] == 0x0 &&
        arppack->arp_tha[4] == 0x0 &&
        arppack->arp_tha[5] == 0x0;
}
