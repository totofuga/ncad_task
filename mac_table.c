#include <search.h>
#include <stdlib.h>
#include <stdio.h>
#include "ncad_task.h"

/* struct hsearch_data mac_table */

/* IntraGuardian2の仕様より */
#define HASH_SIZE 40000 
#define KEY_LENGTH 6*2+1

static char* _generate_key (char *key, const unsigned char *macaddr);
static void _free_key(char *key);

int mac_table_init() {
    hcreate(HASH_SIZE);
}

void mac_table_add(const unsigned char* macaddr){
    char *key = malloc(KEY_LENGTH);
    ENTRY e;   

    key = malloc(KEY_LENGTH);
    _generate_key(key, macaddr);

    e.key  = key;
    e.data = key;
    if(!hsearch(e, ENTER)) {
	perror("mac_table_add error");
	exit(1);
    }
}

int mac_table_has(const unsigned char *macaddr) {
    char key[KEY_LENGTH];
    ENTRY e;   

    _generate_key(key, macaddr);
    e.key  = key;

    return hsearch(e, FIND) != NULL;
}

char* _generate_key (char *key, const unsigned char *macaddr) {
    sprintf(key, "%02X%02X%02X%02X%02X%02X", 
	macaddr[0], 
	macaddr[1], 
	macaddr[2], 
	macaddr[3],
	macaddr[4], 
	macaddr[5]);
    return key;
}

void _free_key(char *key){ free(key); }

#ifdef TEST
#include "test.h"

int main() {
    unsigned char mac_addr1[] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
    unsigned char mac_addr2[] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26};
    char key[KEY_LENGTH];
    _generate_key(key, mac_addr1);
    IS(key, "111213141516");

    mac_table_init();

    OK(!mac_table_has(mac_addr1));
    OK(!mac_table_has(mac_addr2));

    mac_table_add(mac_addr1);

    OK(mac_table_has(mac_addr1));
    OK(!mac_table_has(mac_addr2));

    mac_table_add(mac_addr2);
    OK(mac_table_has(mac_addr1));
    OK(mac_table_has(mac_addr2));
    return 0;
}

#endif
