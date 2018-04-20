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
	info->fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( info->fd == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	struct sockaddr_in servAddr;

	setServerAddress(&servAddr, 0); //set address, port, and add. family.

	if (bindNameToSocket(info->fd, &servAddr) == -1) { // bind address to socket.
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	if(listen(info->fd, 1) == -1) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	if( ( info->port = get_port( info->fd ) ) == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	//send acknowledgment to client with port.
	char buffer[50];
	sprintf(buffer, "A%d\n", info->port);
	unsigned int length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;
	if ( (info->io_fd = accept( info->port, (struct sockaddr *) &clientAddr,
	      &length)) == -1 )
	      send_error(controlfd, ERRNO, NULL);

	if (DEBUG) printf("sent %s", buffer);
	write(controlfd, buffer, strlen(buffer));

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
	if ( filefd == -1 ) return false;
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
	sleep(6);
	close(datafd->io_fd);
	close(datafd->fd);
	return true;
}
