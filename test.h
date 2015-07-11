int is(char *src, char *dst, int line, char* file);
int ok(int isok, int line, char* file);

#define OK(isok) ok(isok, __LINE__, __FILE__)
#define IS(src, dst) is(src, dst, __LINE__, __FILE__)

