#include <stdbool.h>
#include <stdlib.h>

#define PORT            49999
#define CTRL_MSG_SIZE   512
#define DEBUG           0
#define BUFSIZE         512

extern const char *ls_cmd;
extern char *const ls_args[];

extern const char *more_cmd;
extern char *const more_args[];

void quitwitherror();
bool readcontroller( int fd, char *mesg, int buflen );
char* getname( char *path ); //get name from path.
bool chuckfile( int datafd, int filefd );
bool catchfile( int datafd, int filefd );
void debugprint( char* str );
