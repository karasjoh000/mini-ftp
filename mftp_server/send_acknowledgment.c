#include <string.h>
#include <unistd.h>

void send_acknowledgment( int controlfd, int port) {
	char buffer[50];
	if ( port == -1 ) {
		strcpy(buffer, "A\n");
		write(controlfd, buffer, 2);
	} else {
		sprintf(buffer, "A%d\n", port);
		write(controlfd, buffer, strlen(buffer));
	}
}
