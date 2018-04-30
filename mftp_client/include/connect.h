#ifdef CONNECT_H
#define CONNECT_H

/*int create_connection( char* host, int port ) : Connects client to host on 
			port. Returns socket fd if success and -1 if error. */
int create_connection( char* host, int port );

#endif
