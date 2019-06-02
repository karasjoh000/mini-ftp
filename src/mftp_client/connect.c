#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <connect.h>
#include <stdbool.h>

/* configures the connection address that client will connect to. */
void setConnectionAddress( struct sockaddr_in *servAddr, struct hostent* host, int port ) {
  // set to 0 to avoid unwanted configurations.
  memset( ( struct servAddr* ) servAddr, 0, sizeof( servAddr ) );
  servAddr->sin_family = AF_INET; //set family and port.
  servAddr->sin_port = htons( port );
  // set the server address that client will connect to.
  memcpy( &servAddr->sin_addr, *( host->h_addr_list ), sizeof( struct in_addr ) );

}

void print_connection_details(struct hostent* hostEntry, struct sockaddr_in *servAddr) {
    //get ip address of server
    char serverip[INET_ADDRSTRLEN];
    if ( ( inet_ntop( AF_INET, &(servAddr->sin_addr),
                        serverip, INET_ADDRSTRLEN ) ) == NULL )
      strcpy(serverip, "unknown");

    //print connection details to stdout.
    if ( !hostEntry )
      printf( " Connected to [name: unknown][ip: %s] on port \n", serverip );
    printf( "Connected to [name: %s][ip: %s]\n", hostEntry->h_name, serverip );

}

int create_connection( char* host, int port, bool printinfo) {

  int socketfd = socket( AF_INET, SOCK_STREAM, 0 ); // get a socket fd.
  struct sockaddr_in servAddr;
  struct hostent* hostEntry;

  if ( ( hostEntry = gethostbyname( host ) ) == NULL ) { // get struct with host info.
    printf( "no name associated with %s\n", host );
    exit(1);
  }

  setConnectionAddress( &servAddr, hostEntry, port );

  if ( connect( socketfd, ( struct sockaddr * ) &servAddr, /* Connect to server */
                sizeof( servAddr ) ) == -1 ) {
    perror( "Error on connect" );
    return -1;
  }

  if(printinfo) print_connection_details(hostEntry, &servAddr);

  return socketfd;
}
