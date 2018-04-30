#include <stdbool.h>
#include <stdlib.h>

#define PORT            49999 //port that client attempts to connect to.
#define CTRL_MSG_SIZE   512 //Control connection message size limit.
#define DEBUG           0
#define BUFSIZE         512 //buffer size used for arbitrary buffers in program.

/*args used for execvp ls and more. See execvp_args.c*/
extern const char *ls_cmd;
extern char *const ls_args[];

extern const char *more_cmd;
extern char *const more_args[];

/*void quitwitherror() : exits with connection broken error. */
void quitwitherror();

/*bool readcontroller( int fd, char *mesg, int buflen ): read contents on
			control connection to mesg. Return true if
			success and false if error while reading. */
bool readcontroller( int fd, char *mesg, int buflen );

/*char* getname( char *path ) : returns the last element in the path provided. */
char* getname( char *path ); //get name from path.

/*bool chuckfile( int datafd, int filefd ) : send contents of filefd to datafd.
			if error reading filefd, false is returned. If error
			writing to datafd, quitwitherror is called. */
bool chuckfile( int datafd, int filefd );

/*bool catchfile( int datafd, int filefd ) : Reads data from datafd and writes
			it to filefd. If error reading datafd, quitwitherror()
			is called. If error writing to filefd, false is returned.
			If success, true is returned. */
bool catchfile( int datafd, int filefd );

/*void debugprint( char* str ) : prints str with newline at end if DEBUG flag is
			set. */
void debugprint( char* str );
