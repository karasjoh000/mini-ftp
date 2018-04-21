#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void setConnectionAddress(struct sockaddr_in *servAddr, struct hostent* host, int port) {
	// set to 0 to avoid unwanted configurations.
	memset( (struct servAddr*) servAddr, 0, sizeof(servAddr));
	servAddr->sin_family = AF_INET; //set family and port.
	servAddr->sin_port = htons(port);
	// set the server address that client will connect to.
	memcpy(&servAddr->sin_addr, *(host->h_addr_list), sizeof(struct in_addr));

}

int create_connection(char* host, int port) {

	int socketfd = socket( AF_INET, SOCK_STREAM, 0); // get a socket fd.
	struct sockaddr_in servAddr;
	struct hostent* hostEntry;

	if ( ( hostEntry = gethostbyname( host ) ) == NULL )  // get struct with host info.
		printf("no name associated with %s\n", host);

	setConnectionAddress(&servAddr, hostEntry, port);

	if ( connect(socketfd, (struct sockaddr *) &servAddr, /* Connect to server */
	             sizeof(servAddr)) == -1) {
    perror("Error on connect");
    return -1;
  }

  return socketfd;
}
