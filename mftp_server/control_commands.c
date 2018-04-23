#include <control_commands.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <configure_server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <mftp.h>
#include <netdb.h>

#define BUFSIZE 512




void create_data_connection(int controlfd, DATACON* info) {
	debugprint("creating new socket.....");
	info->fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( info->fd == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	struct sockaddr_in servAddr;
	debugprint("got new socket");
	setServerAddress(&servAddr, 0); //set address, port, and add. family.
	debugprint("binding name....");
	if (bindNameToSocket(info->fd, &servAddr) == -1) { // bind address to socket.
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	debugprint("name binded");
	debugprint("setting listen queue...");
	if(listen(info->fd, 1) == -1) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	debugprint("queue set");
	if( ( info->port = get_port( info->fd ) ) == -1 ) {
		send_error(controlfd, ERRNO, NULL);
		return;
	}
	debugprint("got port number");
	//send acknowledgment to client with port.
	char buffer[50];
	sprintf(buffer, "A%d\n", info->port);

	if (DEBUG) printf("sending %s on control connection\n", buffer);
	write(controlfd, buffer, strlen(buffer));

	unsigned int length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	debugprint("waiting for client to accept on data connection...");

	printf("%d\n", info->port);

	if ( (info->io_fd = accept( info->fd, (struct sockaddr *) &clientAddr,
	      &length)) == -1 )
	      send_error(controlfd, ERRNO, NULL);
	debugprint("client connected");

	return;
}


bool changedir(char* path) {
	debugprint("in change dir routine");
  printf("changing to %s\n", path);
	if (chdir(path) == -1) {
    if (DEBUG) perror("Failed changing dir");
    return false;
  }
  debugprint("Successfully changed dir");
	return true;
}




void send_ack(int controlfd, char* str) {
  debugprint("sending ack");
	char msg[CTRL_MSG_SIZE];
	if (!str) strcpy(msg, "A\n");
	else sprintf(msg, "A%s\n", str);
	if (write(controlfd, msg, strlen(msg)) == -1) {
    perror("Fatal error. Connection broken, exiting child server");
    exit(0);
  }
  debugprint("acknowledgment sent.");
	return;
}



void send_error(int clientfd, error_type type, char* str) {
  debugprint("sending error...");
  char mesg[CTRL_MSG_SIZE];
	switch (type) {
		case ERRNO:
      debugprint("In errno case");
      sprintf(mesg, "E%s\n", strerror(errno));
			if (write(clientfd, mesg, strlen(mesg)) == -1 ) {
        if (DEBUG) perror("Connection broken, child server exiting");
        exit(0);
      }
      debugprint("error sent");
			break;
		case HERRNO:
      sprintf(mesg, "E%s\n", hstrerror(h_errno));
			if (write(clientfd, mesg, strlen(mesg)) == -1 ) exit(0);
			break;
		default:
			if ( write( clientfd, str, error_format( str ) ) == -1 ) exit(0);
	}
}



void get(int controlfd, int datafd, char* path) {
  if(DEBUG) printf("in getfile\n");
  int filefd = open (path, O_RDONLY, 0);
  if ( filefd == -1 ) {
  	if (DEBUG) perror("Error opening file to read from");
    close(datafd);
    send_error(controlfd, ERRNO, NULL);
  	return;
  } else send_ack(controlfd, NULL);
  if (!chuckfile(datafd, filefd)) {
    printf("[Error]: Connection broken, child server exiting...\n");
    exit(0);
  }
}




void put(int controlfd, int datafd, char* path) {
    if(DEBUG) printf("creating file %s...", getname(path));
  	int filefd = open(getname(path), O_RDWR | O_CREAT, 0755);
  	if (filefd == -1 ) {
  		close(datafd);
      send_error(controlfd, ERRNO, NULL);
  	} else send_ack(controlfd, NULL);
    if (!catchfile(datafd, filefd)) {
      printf("[Error]: Connection broken, child server exiting...\n");
      exit(0);
    }
}



int error_format(char* str) {
	int len = strlen(str) + 1;
	for( int i = len; i > 0; i-- )
		str[i] = str[i-1];
	str[0] = 'E';
	return len;
}


void ls(int controlfd, int datafd) {
  debugprint("in ls process");
  send_ack(controlfd, NULL);
  if(fork()) {
    close(datafd);
    int stat; wait(&stat);
  } else {
    dup2(datafd, 1);
    close(datafd);
    execvp(ls_cmd, ls_args);
    exit(0);
  }

}
