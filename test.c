#include "test.h"

#include <strings.h>
#include <stdio.h>

int is(char *src, char *dst, int line, char* file) {
    if (! ok(!strcmp(src, dst), line, file )) {
	printf("... [%s] ne [%s]\n", src, dst);
	return 0;
    }
    return 1;
}

int ok(int isok, int line, char* file){
    if(isok){
	printf("OK\n");
	return 1;
    } else {
	printf("NG, line %d, file %s", line, file);
	return 0;
    }
}
