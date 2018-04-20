#ifndef _CONTROL_COMMANDS_H_
#define _CONTROL_COMMANDS_H_

#include <stdbool.h>

typedef struct dataconn {
	int fd;
	int port;
	int io_fd;
} DATACON;

void changedir(int controlfd, char* path);
void create_data_connection(int controlfd, DATACON* info);
bool getfile(int controlfd, DATACON* datac, char* path);

#endif
