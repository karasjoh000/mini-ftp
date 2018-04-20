#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <send_error.h>
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
		if (DEBUG && reads == -1) {
			perror("error on read");
			exit(0);
		}
		int length = strlen(mesg);
		if ( buffer == '\n' ) return true;
		if (DEBUG) printf("Controller read:%c\n", buffer);
		if (length == buflen) {
			send_error(fd, CUST, "Command to big");
			return false;
		}
		mesg[length] = buffer;
		mesg[length + 1] = '\0';
	}
	exit(0);
}
