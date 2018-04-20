#include <control_commands.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <send_error.h>
#include <configure_server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <debug.h>
#include <fcntl.h>
#define BUFSIZE 512


void create_data_connection(int controlfd, DATACON* info) {
	debugprint("creating new socket.....");
	info->fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( info->fd == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	struct sockaddr_in servAddr;
	debugprint("got new socket");
	setServerAddress(&servAddr, 0); //set address, port, and add. family.
	debugprint("binding name....");
	if (bindNameToSocket(info->fd, &servAddr) == -1) { // bind address to socket.
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	debugprint("name binded");
	debugprint("setting listen queue...");
	if(listen(info->fd, 1) == -1) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	debugprint("queue set");
	if( ( info->port = get_port( info->fd ) ) == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	debugprint("got port number");
	//send acknowledgment to client with port.
	char buffer[50];
	sprintf(buffer, "A%d\n", info->port);

	if (DEBUG) printf("sending %s on control connection\n", buffer);
	write(controlfd, buffer, strlen(buffer));

	unsigned int length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	debugprint("waiting for client to accept on data connection...");

	printf("%d\n", info->port); 

	if ( (info->io_fd = accept( info->fd, (struct sockaddr *) &clientAddr, //DOES NOT ACCEPT??
	      &length)) == -1 )
	      send_error(controlfd, ERRNO, NULL);
	debugprint("client connected");

	return;
}


void changedir(int fd, char* path) {
	printf("in chandedir\n");
	if (chdir(path) == -1) {
		send_error(fd, ERRNO, NULL);
	}
	else {
		if ( write(fd, "A\n", 2) == -1 ) {
			printf("Acknowledge error\n"); //quits only if connection
			exit(0);
		}
	}
	return;
}

bool getfile(int controlfd, DATACON* datafd, char* path) {
	if(DEBUG) printf("in getfile\n");
	int reads, filefd = open (path, O_RDONLY, 0);
	if ( filefd == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return false;
	}
	char buffer[BUFSIZE];
	while ( ( reads = read(filefd, buffer, BUFSIZE)) != 0 ) {
		buffer[reads] = '\0';
		if(DEBUG) printf("sending %s\n", buffer);
		if ( write( datafd->io_fd, buffer, reads ) == -1 ) {
			if(DEBUG) printf("recceived error:%s\n", strerror(errno));
			return false;
		}
	}
	printf("file sent\n");
	strcpy(buffer, "A\n");
	write(controlfd, buffer, strlen(buffer) );
	//sleep(6);
	close(datafd->io_fd);
	close(datafd->fd);
	return true;
}
