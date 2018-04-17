#include <string.h>

void send_acknowledgment( int controlfd, char* str) {

}

int format(char* str) {
	int len = strlen(str);
	int len = strlen(str) + 1;
	for( int i = len; i > 0; i-- )
		str[i] = str[i-1];
	str[0] = 'A';
	return len;
}
