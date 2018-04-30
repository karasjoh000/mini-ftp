#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <mftp.h>
#include <stdio.h>
#include <fcntl.h>
#include <responses.h>

void quitwitherror() {
        printf("[PROCESS %d]:", getpid());
        fflush(stdout); //make sure first statment is printed before perror.
        perror( FATAL );
        exit( 1 );
}

bool readcontroller( int fd, char *mesg, int buflen ) {
  debugprint( "reading from controller" );
  char buffer;
  mesg[0] = '\0';
  int reads;
  while ( ( reads = read( fd, &buffer, 1 ) ) != 0 ) {
    if ( reads < 0 ) quitwitherror();
    int length = strlen( mesg );
    if ( buffer == '\n' ) return true; //all messages end with newline so return.
    if ( DEBUG ) printf( "Controller read:%c\n", buffer );
    if ( length == buflen ) return false; //if to big for buffer return false.

    mesg[length] = buffer; //concat an extra char to mesg buffer.
    mesg[length + 1] = '\0';
  } quitwitherror(); //invalid control command assumes broken connection.
}

char* getname( char *path ) { // get the filename from path.
  int len = strlen( path ), i;
  for ( i = len; i >= 0 && path[i] != '/'; --i );
  if ( i == 0 ) return path;
  else return &path[i + 1];
}


bool chuckfile( int datafd, int filefd ) { //sendfile(sockfd, filefd, NULL, BUFSIZE); optimized call.
  if( DEBUG ) printf( "in getfile\n" );
  int reads;
  char buffer[BUFSIZE];
  while ( ( reads = read( filefd, buffer, BUFSIZE ) ) != 0 ) {
    if( reads == -1 ) { //print error and return if cannot read from file.
      perror( E_RD );
      return false;
    }
    if( DEBUG ) printf( "sending %s\n", buffer );
    //if cannot read from datafd, broken connection is assumed.
    if ( write( datafd, buffer, reads ) == -1 ) quitwitherror();
  }
  debugprint( "file sent" );
  return true;
}

bool catchfile( int datafd, int filefd ) {
  char buffer[512];
  int reads;
  debugprint( "reading from network" );
  while ( ( reads = read( datafd, buffer, 512 ) ) != 0 ) {
    if ( reads == -1 ) quitwitherror(); //broken connection
    buffer[reads] = '\0';
    if ( DEBUG ) printf( "data connection read: %s\n", buffer );
    if ( write( filefd, buffer, reads ) == -1 ) { //if cannot write to file return false.
      perror( E_WR );
      return false;
    }
  }
  return true; //on success return true.
}
