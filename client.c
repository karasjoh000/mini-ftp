/*
   John Karasev
   CS 360 Systems Programming
   WSUV Spring 2018
   -----------------------------------------------------
   Assignment #9:
   Using network sockets, make a server and client where client gets time
   from the server. This file is the implementation of the client side.
*/
#include <mftp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <err.h>
#include <errno.h>
#include <debug.h>

#define CTRL_MSG_SIZE 512

//#define PORT 49999
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

	while(!readfromnet(socketfd, buffer, 512));

	if (DEBUG) printf("server response:%s\n", buffer);

	strcpy(buffer, "D\n");
	debugprint("writing D to server");
	if ( write(socketfd, buffer, strlen(buffer)) == -1 ) {
		perror("error on write");
		exit(1);
	}

	while(!readfromnet(socketfd, buffer, 512));

	if (DEBUG) printf("Severs response: %s\n", buffer);

	int data_port;

	sscanf(buffer, "A%d", &data_port);

	int datafd = socket( AF_INET, SOCK_STREAM, 0);

	setConnectionAddress(&servAddr, hostEntry, data_port);

	printf("attempting to connect to %d\n", data_port );
	if ( connect(datafd, (struct sockaddr *) &servAddr, /* Connect to server */ //CONNECTS BUT SERVER NEVER ACCEPTS
		       sizeof(servAddr)) < 0)
		     errx( 1, "error connecting: %s", strerror(errno));
        if (DEBUG) printf("connected to data port\n");

	strcpy(buffer, "G/Users/johnkarasev/Desktop/test.txt\n");
	if (DEBUG) printf("sending:%s", buffer);
	write(socketfd, buffer, strlen(buffer));
	if (DEBUG) printf("sent\n");
	//while(!readfromnet(socketfd, buffer, 512));
	//if (DEBUG) printf("Severs response: %s\n", buffer);
	debugprint("reading from data connection");
	putfile(datafd, "/Users/johnkarasev/Desktop/test.txt");
	//while(!readfromnet(socketfd, buffer, 512));




}
