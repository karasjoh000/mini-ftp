#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <send_error.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <writetoclient.h>

extern int h_errno;

void send_error(int clientfd, error_type type, char* str) {
	switch (type) {
		case ERRNO:
			char *str = strerror(errno);
			writetoclient(clientfd, str, format(str));
			break;
		case HERRNO:
			break;
		default:
			write
	}

}

int void format(char* str) {
	int len = strlen(str) + 1;
	for( int i = len; i > 0; i-- )
		str[i] = str[i-1];
	str[0] = 'E';
	return len;
}
