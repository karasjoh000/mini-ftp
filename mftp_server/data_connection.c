#include <configure_server.h>
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
#include <send_error.h>
#include <changedir.h>
#include <data_connection.h>
#include <configure_server.h>

void create_data_connection(DATACON* info, int clientfd) {
	info->fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( info->fd == -1 ) {
		send_error(clientfd, ERRNO, NULL);
		return;
	}
	struct sockaddr_in servAddr;

	setServerAddress(&servAddr, 0); //set address, port, and add. family.

	if (bindNameToSocket(info->fd, &servAddr) == -1) // bind address to socket.
		send_error(clientfd, ERRNO, NULL);
	if(listen(info->fd, 1) == -1)
		send_error(clientfd, ERRNO, NULL);
	if( ( info->port = get_port( info->fd ) ) == -1 )
		send_error(clientfd, ERRNO, NULL);
	return;
}

void notifyServer(char* stre, int length, int* fd ) {
	close(fd[0]);
	write(fd[1], stre, length);
	close(fd[1]);
}





void childserver( int* pipefd ) {

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) errx( 1, "error getting socket fd: %s", strerror(errno));

	struct sockaddr_in servAddr;

	setServerAddress(&servAddr, 0); //set address, port, and add. family.

	bindNameToSocket(listenfd, &servAddr); // bind address to socket.

	if(listen(listenfd, 1) == -1)
		errx( 1, "error setting connections queue: %s", strerror(errno));

	unsigned int length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	int connectfd;

	struct hostent* hostEntry;

	int port = get_port(listenfd);

	if ( port == -1 ) {
		char* stre = strerror(errno);
		notifyServer(stre, error_format(stre), pipefd);
		exit(1);
	} else {
		char portstr[10];
		sprintf(portstr, "%d", port);
		int len = strlen(portstr);
		sprintf(portstr, "A%d\n", port);
		notifyServer(portstr, len + 2, pipefd);
	}

	printf("listing on port %d\n", port);

	if ( (connectfd = accept( listenfd, (struct sockaddr *) &clientAddr, &length)) == -1 )
		errx( 1, "error connecting: %s", strerror(errno));

	char buffer[500];
	int bytes;
	if ( (bytes = read(connectfd, buffer, 500)) != 0 ) {
		for ( int i = 0; i < bytes; i++ ) {
			if ( buffer[i] == '\n'){
				printf("\n");
				break;
			}
			else printf("%c", buffer[i]);
		}
	}
	buffer[bytes] = '\0';
	char path[100];
	sscanf(buffer, "C%s\n", path);
	changedir(connectfd, path);


	exit(0);

}
