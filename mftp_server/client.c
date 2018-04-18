/*
   John Karasev
   CS 360 Systems Programming
   WSUV Spring 2018
   -----------------------------------------------------
   Assignment #9:
   Using network sockets, make a server and client where client gets time
   from the server. This file is the implementation of the client side.
*/

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
#include <err.h>
#include <errno.h>
#include <debug.h>
#include <send_error.h>

#define CTRL_MSG_SIZE 512

#define PORT 49999
/* Configures the address of the server. */
void setConnectionAddress(struct sockaddr_in *servAddr, struct hostent* host, int port) {
	// set to 0 to avoid unwanted configurations.
	memset( (struct servAddr*) servAddr, 0, sizeof(servAddr));
	servAddr->sin_family = AF_INET; //set family and port.
	servAddr->sin_port = htons(port);
	// set the server address that client will connect to.
	memcpy(&servAddr->sin_addr, *(host->h_addr_list), sizeof(struct in_addr));

}


int main (int argc, char** argv) {

	int socketfd = socket( AF_INET, SOCK_STREAM, 0); // get a socket fd.
	struct sockaddr_in servAddr;
	struct hostent* hostEntry;

	if( argc == 1 )
		errx(0, "please provide a hostname in arguments");


	if ( ( hostEntry = gethostbyname( argv[1] ) ) == NULL )  // get struct with host info.
		errx( 1, "no name associated with %s\n", argv[1]);

	setConnectionAddress(&servAddr, hostEntry, PORT);

	if ( connect(socketfd, (struct sockaddr *) &servAddr, /* Connect to server */
	             sizeof(servAddr)) == -1)
		errx( 1, "error connecting: %s", strerror(errno));

	char controlmesg[512];
	char buffer[512];

	strcpy(buffer, "C..\n"); // This will cause error (this is for testing the send_error())
	write(socketfd, buffer, strlen(buffer));

	int bytes_read;

	while ( ( bytes_read = read(socketfd, buffer, CTRL_MSG_SIZE) ) != 0) {
		if ( ( strlen(controlmesg) + bytes_read ) >=  CTRL_MSG_SIZE ) {
			send_error(socketfd, CUST, "Command to large for server buffer");
		}
		for ( int i = strlen(controlmesg), j = 0; j < bytes_read; i++, j++ ) {
			if ( buffer[j] == '\n') {
				controlmesg[i] = '\0';
				break;
			}
			controlmesg[i] = buffer[j];
		}
	}

	printf("server response:%s", buffer);

	strcpy(buffer, "D\n");
	write(socketfd, buffer, strlen(buffer));


	int datafd = socket( AF_INET, SOCK_STREAM, 0);

	setConnectionAddress(&servAddr, hostEntry, controller_port);

	printf("attempting to connect to %d\n", controller_port );
	if ( connect(datafd, (struct sockaddr *) &servAddr, /* Connect to server */
		       sizeof(servAddr)) == -1)
		     errx( 1, "error connecting: %s", strerror(errno));






}
