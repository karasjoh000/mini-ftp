#define CD       0 //update the hash function if this is changed.
#define GET      1
#define PUT      2
#define LS       3
#define SHOW     4
#define RCD      5
#define RLS      6
#define EXIT     7

#define SPLIT    " "

int hash(char*);
bool checkargs(char *cmdline, int cmd);
void stripchar(char* str, char ch);
bool *getpath(char* path, char* cmdline);
