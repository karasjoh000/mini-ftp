#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <send_error.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <debug.h>

int error_format(char* str) {
	debugprint("formatting error string");
	int len = strlen(str);
	for( int i = len; i > 0; i-- ) {
		str[i] = str[i-1];
	}
	str[0] = 'E';
	debugprint("finished error string format");
	return len;
}

void send_error(int clientfd, error_type type, char* str) {
	debugprint("in error");
	char buffer[50];
	switch (type) {
		case ERRNO:
			strcpy(buffer, strerror(errno));
			debugprint("in errno case");
			if (write(clientfd, buffer, error_format(buffer)) == -1 ) {
				debugprint("error writing error");
				exit(0);
			}
			break;
		case HERRNO:
			debugprint("in herrno case");
			strcpy(buffer, hstrerror(h_errno));
			if (write(clientfd, buffer, error_format(buffer)) == -1 ) {
				debugprint("error writing error");
				exit(0);
			}
			break;
		default:
			debugprint("in default errno case");
			strcpy(buffer, str);
			if ( write( clientfd, buffer, error_format( buffer ) ) == -1 ) {
				debugprint("error writing error");
				exit( 0 );

			}
	}
	debugprint("error message sent");

}
