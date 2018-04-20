#include <stdbool.h>

#define PORT            49999
#define CTRL_MSG_SIZE   512
#define DEBUG           1


bool readfromnet(int fd, char *mesg, int buflen);
char* getname(char *path);
bool getfile(int datac, char* path);
bool putfile(int datafd, char* path);
void debugprint(char* str);
