#include <writetoclient.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void writetoclient(int clientfd, char* str, int len) {
	if(write(clientfd, str, len) == -1) exit(0);
}
