#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <send_error.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>


//extern int h_errno;

void send_error(int clientfd, error_type type, char* str) {
	switch (type) {

		case ERRNO:
			str = strerror(errno);
			if (write(clientfd, str, error_format(str)) == -1 ) exit(0);
			break;
		case HERRNO:
			str = hstrerror(h_errno);
			if (write(clientfd, str, error_format(str)) == -1 ) exit(0);
			break;
		default:
			if ( write( clientfd, str, error_format( str ) ) == -1 ) exit( 0 );
	}

}

int error_format(char* str) {
	int len = strlen(str) + 1;
	for( int i = len; i > 0; i-- )
		str[i] = str[i-1];
	str[0] = 'E';
	return len;
}
