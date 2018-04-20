#include <stdbool.h>

#define PORT 49999

bool readfromnet(int fd, char *mesg, int buflen);
char* getname(char *path);
