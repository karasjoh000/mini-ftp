#include <stdbool.h>

#define PORT            49999
#define CTRL_MSG_SIZE   512
#define DEBUG           1


bool readfromnet(int fd, char *mesg, int buflen);
char* getname(char *path); //get name from path.
bool chuckfile(int datafd, int filefd);
bool catchfile(int datafd, int filefd);
void debugprint(char* str);
