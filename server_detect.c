/* 
 とりあえずsendmail経由でメールを送る
 環境が用意出来なかったのでちゃんと送信するテストは行っていません
 本文の出力のみ確認
*/

#include "ncad_task.h"
#include <stdio.h>
#include <stdlib.h>

extern struct ncad_task_context context;

void _mail_send(const char *title, const char *body);
char* _make_body(char* buf, const struct ether_arp *arppack);

void server_detect(const struct ether_arp *arppack) {

    char buf[255];

    _mail_send("find bad address!!", _make_body(buf, arppack));
}

char* _make_body(char* buf, const struct ether_arp *arppack) {

    sprintf(buf, "IP Addr: %3d.%3d.%3d.%3d\nMAC Addr: %02x:%02x:%02x:%02x:%02x:%02x\n", 
        arppack->arp_spa[0], /* IP */
        arppack->arp_spa[1],
        arppack->arp_spa[2],
        arppack->arp_spa[3],
        arppack->arp_sha[0], /* MAC*/
        arppack->arp_sha[1],
        arppack->arp_sha[2],
        arppack->arp_sha[3],
        arppack->arp_sha[4],
        arppack->arp_sha[5]
    );
    return buf;
}


void _mail_send(const char *title, const char *body)  {
    FILE* fp;
    if ((fp = popen("/usr/lib/sendmail -t", "w")) == NULL){
        perror("/usr/lib/sendmail popen error \n");
        exit(1);
    }

    /* 送信元 */
    fputs("From: ", fp);
    fputs(context.email_from, fp);
    putc('\n', fp);

    /* 宛先 */
    fputs("To: \n", fp);
    fputs(context.email_to, fp);
    putc('\n', fp);

    /* タイトル */
    fputs("Subject: ", fp);
    fputs(title, fp);
    putc('\n', fp);

    putc('\n', fp);

    /* 本文 */
    fputs(body, fp);
    fputs("\n.\n", fp);
    pclose(fp);
}


#ifdef TEST
#include "test.h"

struct ncad_task_context context;

int main() {
    char buf[255];
    struct ether_arp arppack;

    arppack.arp_spa[0] = 111; /* IP */
    arppack.arp_spa[1] = 112;
    arppack.arp_spa[2] = 113;
    arppack.arp_spa[3] = 114;
    
    arppack.arp_sha[0] = 0x11; /* MAC*/
    arppack.arp_sha[1] = 0x12;
    arppack.arp_sha[2] = 0x13;
    arppack.arp_sha[3] = 0x14;
    arppack.arp_sha[4] = 0x15;
    arppack.arp_sha[5] = 0x16;

    IS(_make_body(buf, &arppack), "IP Addr: 111.112.113.114\nMAC Addr: 11:12:13:14:15:16\n");
    return 0;
}

#endif
