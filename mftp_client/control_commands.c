#include <string.h>
#include <mftp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <control_commands.h>
#include <connect.h>

bool isError(char* response) {
	if(response[0] == 'E') {
		printf("Error from server: %s\n", &response[1]);
    return true;
  }
  return false;
}

void rcd(int fd, char* path) {
  debugprint("in rcd");
  if (DEBUG) printf("remote cd path:%s\n", path);
  char mesg[CTRL_MSG_SIZE];
	sprintf(mesg, "C%s\n", path);
  if(DEBUG) printf("Sending to controller: %s\n", mesg);
	if ( write(fd, mesg, strlen(mesg)) == -1 ) {
    perror("[ERROR]: Error sending command to controller");
    return;
  }
	while(!readfromnet(fd, mesg, CTRL_MSG_SIZE)) {
		printf("[ERROR]: Failed to read acknowledgement from server\n");
    return;
  }
  if(isError(mesg)) return;
}

void cd(char* path) {
	debugprint("in change dir routine");
	if (chdir(path) == -1) perror("[ERROR]: Error changing directory");
}

void get(int controlfd, char* path, char* host) {
  char mesg[CTRL_MSG_SIZE];
  int datafd;

  if(DEBUG) printf("creating %s...\n", path);
  int filefd = open (getname(path), O_RDWR | O_CREAT, 0755);
  if ( filefd == -1 ) {
  	perror("[ERROR]: Cannot open file, check your permissions");
    return;
  }

  debugprint("creating data connection...");
  if ( (datafd = createdatac(controlfd, host) ) == -1) {
    printf("Failed create data connection with server\n");
    return;
  }
  debugprint("[SUCCESS]: Data connection created");

  sprintf(mesg, "G%s\n", path);
  if (DEBUG) printf("sending over controller: %s", mesg);
  if (write(controlfd, mesg, strlen(mesg)) == -1) {
    perror("[FATAL ERROR]: Connection broken, exiting.");
  }

  if(!readfromnet(controlfd, &mesg, CTRL_MSG_SIZE)) {
    printf("[ERROR]: failed to read acknowledgement from server\n");
    close(datafd);
    return;
  }

  if(isError(mesg)) {
    close(datafd);
    return;
  }

  if (!catchfile(datafd, filefd))
    perror("Error retreiving file");
}

void put(int controlfd, char* path, char* host) {
  char mesg[CTRL_MSG_SIZE];

  int filefd = open (path, O_RDONLY, 0);
  if ( filefd == -1 ) {
    perror("[Error]: Cannot open file for reading");
    return;
  }
  int datafd = createdatac(controlfd, host);
  if ( datafd == -1 ) {
    printf("[ERROR]: failed to create data connection with server\n");
    return;
  }
  debugprint("[SUCCESS]: Data connection created");
  sprintf(mesg, "P%s\n", path);
  if(DEBUG) printf("[SENDING]: sending over controller: %s ", mesg);
  if (write(controlfd, mesg, strlen(mesg)) == -1) {
    perror("[FATAL ERROR]: Connection broken, exiting.");
  }
  if(isError(mesg)) {
    close(datafd);
    return;
  }

  if(!chuckfile(datafd, filefd)) {
    perror("[ERROR]: Error sending file, probably server is down\n");
    close(datafd);
    return;
  }
  debugprint("[SUCCESS]: File sent to server");

}

int createdatac(int controlfd, char* host) {
    char mesg[CTRL_MSG_SIZE];
    if(DEBUG) printf("controlfd: %d", controlfd);
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
