#include <changedir.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <send_error.h>


char* changedir(int fd, char* path) {
	if (chdir(path) == -1) {

	}
	return;
}
