#include <changedir.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <send_error.h>


void changedir(int fd, char* path) {
	if (chdir(path) == -1) send_error(fd, ERRNO, NULL);
	else { 
		if ( write(fd, "A\n", 2) == -1 ) exit(0);
	}
	return;
}
