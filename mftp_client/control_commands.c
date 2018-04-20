#include <string.h>
#include <mftp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <control_commands.h>
#include <connect.h>

void rcd(int fd, char* path) {
  char mesg[CTRL_MSG_SIZE];
	sprintf(mesg, "C%s", path);
	write(fd, mesg, strlen(mesg));
	if(!readfromnet(fd, &mesg, CTRL_MSG_SIZE)) {
		printf("failed to read acknowledgement from server\n");
    return;
  }
	if(mesg[0] == 'A')
		return;
	if(mesg[0] == 'E')
		printf("Error from server: %s\n", &mesg[1]);
}

void cd(char* path) {
	debugprint("in change dir routine");
	if (chdir(path) == -1) perror("Error changing directory");
}

void get(int controlfd, char* path, char* host) {
  int datafd;
  if ( (datafd = createdatac(host) ) == -1) {
    printf("Failed create data connection with server\n");
    return;
  }
  if (!getfile(datafd, path))
    perror("Error retreiving file");
}

void put(int controlfd, char* path, char* host) {
  int datafd = createdatac(host);
  if ( datafd == -1 ) {
    printf("failed to create data connection with server\n");
    return;
  }
  if(!putfile(datafd, path))
    perror("Error sending file\n");
}

int createdatac(int controlfd, char* host) {
    char mesg[CTRL_MSG_SIZE];
    if (write(controlfd, "D\n", 2) == -1)  {
      perror("Error on write to server");
      printf("Connection with server broken. Exiting...\n");
      exit(0);
    }
    if(!readfromnet(controlfd, &mesg, CTRL_MSG_SIZE)) {
      printf("failed to read acknowledgement from server\n");
      return -1;
    }
    int port;
    if (sscanf(mesg, "A%d", &port) != 1) return -1;
    return create_connection(host, port);
}
