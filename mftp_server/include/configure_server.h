#ifndef CONFIGURE_SERVER_H
#define CONFIGURE_SERVER_H

void killZombies();
void releaser( void* p );
void serveClient( int netfd );
void setServerAddress( struct sockaddr_in* servAddr, int port );
int bindNameToSocket( int listenfd, struct sockaddr_in* servAddr );
int get_port( int socketfd );

#endif
