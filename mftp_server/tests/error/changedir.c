#include <changedir.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <send_error.h>
#include <debug.h>

void changedir(int fd, char* path) {
	char buffer[20];
	strcpy(buffer, "error\n");
	debugprint("in change dir");
	if (chdir(path) == -1) {
		debugprint("error changing dir");
		//write(fd, buffer, strlen(buffer));
		send_error(fd, ERRNO, NULL);
	}
	else {
		debugprint("success, writing success");
		if ( write(fd, "A\n", 2) == -1 ) exit(0);
		debugprint("sent acknowlegement to client");
	}
	return;
}
