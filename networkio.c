#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <debug.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 512

bool readfromnet(int fd, char *mesg, int buflen) {
	debugprint("reading from controller");
	char buffer;
	mesg[0] = '\0';
	int reads;
	while ( ( reads = read(fd, &buffer, 1) ) != 0 ) {
		if ( reads == -1) {
			perror("error on read, child server exiting");
			exit(0);
		}
		int length = strlen(mesg);
		if ( buffer == '\n' ) return true;
		if (DEBUG) printf("Controller read:%c\n", buffer);
		if (length == buflen) return false;

		mesg[length] = buffer;
		mesg[length + 1] = '\0';
	}
	exit(0);
}

char* getname(char *path) {
	int len = strlen(path), i;
	for ( i = len; i >= 0 && path[i] != '/'; --i );
	if ( i == 0 ) return path;
	else return &path[i+1];

}

bool getfile(int datafd, char* path) {
	if(DEBUG) printf("in getfile\n");
	int reads, filefd = open (path, O_RDONLY, 0);
	if ( filefd == -1 ) {
		if (DEBUG) perror("Error opening file to write to");
		return false;
	}
	char buffer[BUFSIZE];
	while ( ( reads = read(filefd, buffer, BUFSIZE)) != 0 ) {
		buffer[reads] = '\0';
		if(DEBUG) printf("sending %s\n", buffer);
		if ( write( datafd, buffer, reads ) == -1 ) {
			if(DEBUG) perror("Error when writing to file form net");
			return false;
		}
	}
	debugprint("file sent");
	close(filefd); close(datafd);
	return true;
}

bool putfile(int datafd, char* path) {
	char buffer[512];
	int filefd = open(getname(path), O_RDWR | O_CREAT, 0755), reads;
	if (filefd == -1 ) {
		printf("Error on open");
		exit(1);
	}
	printf("reading from network\n");
	while ( (reads = read(datafd, buffer, 512) ) != 0 ) {
		if (reads == -1 ) return false;
		buffer[reads] = '\0';
		if (DEBUG) printf("data connection read: %s\n", buffer);
		if ( write(filefd, buffer, reads) == -1) return false;
	}
	close(filefd); close(datafd);
	return true;
}
