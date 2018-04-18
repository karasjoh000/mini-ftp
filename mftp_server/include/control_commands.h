#ifndef _CONTROL_COMMANDS_H_
#define _CONTROL_COMMANDS_H_

typedef struct dataconn {
	int fd;
	int port;
} DATACON;

void changedir(int controlfd, char* path);
void create_data_connection(int controlfd, DATACON* info);

#endif
