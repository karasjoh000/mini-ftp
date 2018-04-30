#ifndef CONFIGURE_SERVER_H
#define CONFIGURE_SERVER_H

/*void setServerAddress( struct sockaddr_in* servAddr, int port ) : sets
			the feilds of sockaddr_in to the correct address, port,
			and address family */
void setServerAddress( struct sockaddr_in* servAddr, int port );

/*int bindNameToSocket( int listenfd, struct sockaddr_in* servAddr ) : binds
			a name to listenfd socket fd. Returns 0 on success and
			-1 on failure. */
int bindNameToSocket( int listenfd, struct sockaddr_in* servAddr );

/*int get_port( int socketfd ) :  Finds the port number of socketfd fd.
 			Returns -1 on error and port number on success. */
int get_port( int socketfd );

#endif
