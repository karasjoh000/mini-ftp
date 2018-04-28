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
    if ( buffer == '\n' ) return true;
    if ( DEBUG ) printf( "Controller read:%c\n", buffer );
    if ( length == buflen ) return false;

    mesg[length] = buffer;
    mesg[length + 1] = '\0';
  }
  quitwitherror();
  return false;
}

char* getname( char *path ) {
  int len = strlen( path ), i;
  for ( i = len; i >= 0 && path[i] != '/'; --i );
  if ( i == 0 ) return path;
  else return &path[i + 1];
}
//alternative -> send open fd over socket
bool chuckfile( int datafd, int filefd ) { //sendfile(sockfd, filefd, NULL, BUFSIZE); optimized call.
  if( DEBUG ) printf( "in getfile\n" );
  int reads;
  char buffer[BUFSIZE];
  while ( ( reads = read( filefd, buffer, BUFSIZE ) ) != 0 ) {
    if( reads == -1 ) {
      perror( E_RD );
      return false;
    }
    buffer[reads] = '\0';
    if( DEBUG ) printf( "sending %s\n", buffer );
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
    if ( reads == -1 ) quitwitherror();
    buffer[reads] = '\0';
    if ( DEBUG ) printf( "data connection read: %s\n", buffer );
    if ( write( filefd, buffer, reads ) == -1 ) {
      perror( E_WR );
      return false;
    }
  }
  return true;
}
