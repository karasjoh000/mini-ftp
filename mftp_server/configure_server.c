#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <err.h>
#include <errno.h>
#include <configure_server.h>


// Forked process exectues this function, sends date/time to client then exits.
void serveClient( int netfd ) {
  char buffer[40];
  time_t current = time( NULL );
  strcpy( buffer, ctime( &current ) );
  write( netfd, buffer, strlen( buffer ) + 1 ); //+1 for null terminator.
  close( netfd );
  exit( 0 );
}

// configures server address (Port, family, and address).
void setServerAddress( struct sockaddr_in* servAddr, int port ) {
  memset( servAddr, 0, sizeof( *servAddr ) );
  servAddr->sin_family = AF_INET;
  servAddr->sin_port = htons( port );
  servAddr->sin_addr.s_addr = htonl( INADDR_ANY );

  return;
}

// when name is configured, it needs to be binded to the socket.
// listedfd -> socket fd
// servAddr -> struct that containes address configs.
int bindNameToSocket( int listenfd, struct sockaddr_in* servAddr ) {
  int on = 1;
  // allow reuse of socket without waiting 3 min before using again.
  if ( setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof( on ) ) < 0 )
    return -1;
  if ( bind( listenfd, /*assigns a name to an unnamed socket*/
             ( struct sockaddr * ) servAddr, sizeof( *servAddr ) ) < 0 ) {
    return -1;
  }
  return 0;

}

int get_port( int socketfd ) {

  struct sockaddr_in childname;

  socklen_t addrlen = ( socklen_t )sizeof( childname );

  memset( &childname, 0, sizeof( childname ) );

  if ( getsockname( socketfd, &childname, &addrlen ) == -1 ) {
    return -1;
  }

  return ntohs( childname.sin_port );
}
