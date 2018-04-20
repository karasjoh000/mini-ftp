#define CD       0 //update the hash function if this is changed.
#define GET      1
#define PUT      2
#define LS       3
#define SHOW     4
#define RCD      5
#define RLS      6
#define EXIT     7

char* readline();
int hash(char*);
bool checkargs(char* line, char* split, int cmd);
