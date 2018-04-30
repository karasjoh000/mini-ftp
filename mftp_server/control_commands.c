#include <control_commands.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <configure_server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <mftp.h>
#include <netdb.h>
#include <responses.h>

#define BUFSIZE 512


void create_data_connection( int controlfd, DATACON* info ) {
  debugprint( "creating new socket....." );
  if(SERVER_PRINT) printf("[PROCESS %d]: Creating data connection\n", getpid());
  info->fd = socket( AF_INET, SOCK_STREAM, 0 );
  if ( info->fd == -1 ) {
    send_error( controlfd, ERRNO, NULL );
    return;
  }
  struct sockaddr_in servAddr;
  debugprint( "got new socket" );
  setServerAddress( &servAddr, 0 ); //set address, port, and add. family.
  debugprint( "binding name...." );
  if ( bindNameToSocket( info->fd, &servAddr ) == -1 ) { // bind address to socket.
    send_error( controlfd, ERRNO, NULL );
    return;
  }
  debugprint( "name binded" );
  debugprint( "setting listen queue..." );
  if( listen( info->fd, 1 ) == -1 ) {
    send_error( controlfd, ERRNO, NULL );
    return;
  }
  debugprint( "queue set" );
  if( ( info->port = get_port( info->fd ) ) == -1 ) {
    send_error( controlfd, ERRNO, NULL );
    return;
  }
  debugprint( "got port number" );
  //send acknowledgment to client with port.
  char buffer[50];
  sprintf( buffer, "%d", info->port );

  if ( DEBUG ) printf( "sending %s on control connection\n", buffer );
  send_ack(controlfd, buffer);

  unsigned int length = sizeof( struct sockaddr_in );
  struct sockaddr_in clientAddr;

  debugprint( "waiting for client to accept on data connection..." );

  if ( ( info->io_fd = accept( info->fd, ( struct sockaddr * ) &clientAddr,
                               &length ) ) == -1 ) {
    send_error( controlfd, ERRNO, NULL );
    return;
  }
  debugprint( "client connected" );

  return;
}


bool changedir( char* path ) {
  debugprint( "in change dir routine" );
  if(SERVER_PRINT) printf( "[PROCESS %d]: changing to %s\n", getpid(), path );
  if ( chdir( path ) == -1 ) {
    if ( DEBUG ) perror( "Failed changing dir" );
    return false;
  }
  debugprint( "Successfully changed dir" );
  return true;
}




void send_ack( int controlfd, char* str ) {
  debugprint( "sending ack" );
  char msg[CTRL_MSG_SIZE];
  if ( !str ) strcpy( msg, "A\n" );
  else sprintf( msg, "A%s\n", str );
  if ( write( controlfd, msg, strlen( msg ) ) == -1 ) quitwitherror();
  if(SERVER_PRINT) printf("[PROCESS %d]: Sent Ack %s", getpid(), msg);
  return;
}



void send_error( int clientfd, error_type type, char* str ) {
  debugprint( "sending error..." );
  char mesg[CTRL_MSG_SIZE];
  switch ( type ) {
  case ERRNO:
    sprintf( mesg, "E%s\n", strerror( errno ) );
    break;
  case HERRNO:
    sprintf( mesg, "E%s\n", hstrerror( h_errno ) );
    break;
  case CUST:
    sprintf(mesg, "E%s\n", str);
    break;
  default:
    return;
  }
  if ( write( clientfd, mesg, strlen( mesg ) ) == -1 ) quitwitherror();
  if (SERVER_PRINT) printf("[PROCESS %d]: Sent error %s", getpid(), mesg);

}



void get( int controlfd, int datafd, char* path ) {
  if( DEBUG ) printf( "in getfile\n" );
  if(SERVER_PRINT) printf("[PROCESS %d]: opening file %s for reading\n", getpid(), path);
  int filefd = open ( path, O_RDONLY, 0 );
  if ( filefd == -1 ) {
    if ( DEBUG ) perror( "Error opening file to read from" );
    close( datafd );
    send_error( controlfd, ERRNO, NULL );
    return;
  } else send_ack( controlfd, NULL );
  chuckfile( datafd, filefd );
  close(datafd);
  close(filefd);
}




void put( int controlfd, int datafd, char* path ) {
  if( DEBUG ) printf( "creating file %s...", getname( path ) );
  if( SERVER_PRINT ) printf("[PROCESS %d]: creating file %s for writing\n", getpid(), path);
  int filefd = open( getname( path ), O_RDWR | O_CREAT, 0755 );
  if ( filefd == -1 ) {
    close( datafd );
    send_error( controlfd, ERRNO, NULL );
  } else send_ack( controlfd, NULL );
  catchfile( datafd, filefd );
  close(datafd);
  close(filefd);
}



int error_format( char* str ) {
  int len = strlen( str ) + 1;
  for( int i = len; i > 0; i-- )
    str[i] = str[i - 1];
  str[0] = 'E';
  return len;
}


void ls( int controlfd, int datafd ) {
  printf("[PROCESS %d]: executing ls\n", getpid());
  debugprint( "in ls process" );
  send_ack( controlfd, NULL );
  if( fork() ) {
    close( datafd );
    int stat;
    wait( &stat );
  } else {
    dup2( datafd, 1 );
    close( datafd );
    execvp( ls_cmd, ls_args );
    exit( 0 );
  }

}
