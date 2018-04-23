#include <string.h>
#include <mftp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <control_commands.h>
#include <connect.h>
#include <responses.h>
#include <sys/wait.h>

//TODO find why -l not working on ls.

const char *ls_cmd;
const char *ls_args[]; //= {"-l", (void*)NULL};

const char *more_cmd;
const char *more_args[]; //= {"-20", (void*)NULL};


bool isError(char* response) {
	if(response[0] == 'E') {
		printf(E_SERV, &response[1]);
    return true;
  }
  return false;
}

void quit(int controlfd) {
  char mesg[CTRL_MSG_SIZE];
  strcpy(mesg, "Q\n");
  if ( write(controlfd, mesg, CTRL_MSG_SIZE) == -1) {
    perror(FATAL);
    exit(0);
  }
  if (!read_ack(controlfd)) return;
  exit(0);
}


void rcd(int fd, char* path) {
  debugprint("in rcd");
  if (DEBUG) printf("remote cd path:%s\n", path);
  char mesg[CTRL_MSG_SIZE];
	sprintf(mesg, "C%s\n", path);
  if(DEBUG) printf("Sending to controller: %s\n", mesg);
	if ( write(fd, mesg, strlen(mesg)) == -1 ) {
    perror(E_CTRL);
    return;
  }
	while(!readcontroller(fd, mesg, CTRL_MSG_SIZE)) {
		printf(E_ACK);
    return;
  }
  if(isError(mesg)) return;
}



bool cd(char* path) {
	debugprint("in change dir routine");
	if (chdir(path) == -1) {
    perror(E_CHDIR);
    return false;
  }
  return true;
}




void get(int controlfd, char* path, char* host) {
  char mesg[CTRL_MSG_SIZE];
  int datafd;

  if(DEBUG) printf("creating %s...\n", path);
  int filefd = open (getname(path), O_RDWR | O_CREAT, 0755);
  if ( filefd == -1 ) {
  	perror(E_OPEN);
    return;
  }

  debugprint("creating data connection...");
  if ( (datafd = createdatac(controlfd, host) ) == -1) {
    printf(E_DATAC);
    return;
  }
  debugprint("[SUCCESS]: Data connection created");

  sprintf(mesg, "G%s\n", path);
  if (DEBUG) printf("sending over controller: %s", mesg);
  if (write(controlfd, mesg, strlen(mesg)) == -1) {
    perror(FATAL);
    exit(0);
  }

  if(!readcontroller(controlfd, &mesg, CTRL_MSG_SIZE)) {
    printf(E_ACK);
    close(datafd);
    return;
  }

  if(isError(mesg)) {
    close(datafd);
    return;
  }

  if (!catchfile(datafd, filefd))
    return;
}





void put(int controlfd, char* path, char* host) {
  char mesg[CTRL_MSG_SIZE];

  int filefd = open (path, O_RDONLY, 0);
  if ( filefd == -1 ) {
    perror(E_OPEN);
    return;
  }
  int datafd = createdatac(controlfd, host);
  if ( datafd == -1 ) {
    printf(E_DATAC);
    return;
  }
  debugprint("[SUCCESS]: Data connection created");
  sprintf(mesg, "P%s\n", path);
  if(DEBUG) printf("[SENDING]: sending over controller: %s ", mesg);
  if (write(controlfd, mesg, strlen(mesg)) == -1) {
    perror(FATAL);
    exit(1);
  }

  if(!readcontroller(controlfd, &mesg, CTRL_MSG_SIZE)) {
    printf(E_ACK);
    close(datafd);
    return;
  }

  if(isError(mesg)) {
    close(datafd);
    return;
  }

  if(!chuckfile(datafd, filefd)) {
    close(datafd);
    return;
  }
  debugprint("[SUCCESS]: File sent to server");

}




int createdatac(int controlfd, char* host) {
    char mesg[CTRL_MSG_SIZE];
    if(DEBUG) printf("controlfd: %d", controlfd);
    if (write(controlfd, "D\n", 2) == -1)  {
      perror(FATAL);
      exit(0);
    }
    if(!readcontroller(controlfd, &mesg, CTRL_MSG_SIZE)) {
      printf(E_ACK);
      return -1;
    }
    int port;
    if (sscanf(mesg, "A%d", &port) != 1) return -1;
    return create_connection(host, port);
}

bool read_ack(int controlfd) {
  char mesg[CTRL_MSG_SIZE];
  if(!readcontroller(controlfd, &mesg, CTRL_MSG_SIZE)) {
      printf(E_ACK);
      return false;
  }
  if(isError(mesg)) return false;
  return true;
}


void printcontents(int controlfd, print_type type, char* path, char* host) {
  if (DEBUG) printf("LS ARGS: %s %s\n", ls_args[0], ls_args[1]);
  if (DEBUG) printf("MORE ARGS: %s %s\n", more_args[0], more_args[1]);
  int datafd;
  char mesg[CTRL_MSG_SIZE];
  if ( type == PRINTSHOW || type == PRINTRLS ) {
    if ( (datafd = createdatac(controlfd, host) ) == -1) {
        printf(E_DATAC);
        exit(1);
    }
    switch (type) {
      case PRINTRLS:
          strcpy(mesg, "L\n"); break;
      case PRINTSHOW:
          sprintf(mesg, "G%s\n", path); break;
    }
    if(write(controlfd, mesg, strlen(mesg)) == -1) {
        perror(FATAL);
        exit(1);
    }
    if(!read_ack(controlfd)){
        printf(E_ACK); close(datafd); exit(1);
    }

  }

  if (fork()) {
    int status; wait(&status);
    return;
  } else {
    debugprint("In more process");
    more(controlfd, datafd, type, path, host);
  }
}


void more(int controlfd, int datafd, print_type type, char *path, char *host) {
  int fd[2];
  pipe(fd);

  if( fork() ) {
		close( fd[1] );
		int status; wait( &status );
		close( 0 ); dup( fd[0] ); close( fd[0] );
		execvp( more_cmd, more_args);
		exit ( 2 );
  } else {
    close( fd[0] );
    close( 1 ); dup( fd[1] ); close( fd[1] );
    switch(type) {
      case PRINTLS:
        ls(); break;
      case PRINTRLS:
      case PRINTSHOW:
        servprint(controlfd, datafd, host); break;
      default:
        debugprint("NO CASE MATCH");
    }
    exit(2);
  }
}


void ls() {
  const char *args[] = {"-l", NULL};
  execvp(ls_cmd, args);
  printf("Failed to execute ls\n");
  exit(2);
}

void servprint(int controlfd, int datafd, char* host) {
  debugprint("in servprint");
  char mesg[CTRL_MSG_SIZE];
  int reads;

  while( (reads = read(datafd, mesg, CTRL_MSG_SIZE)) != 0) {
    if ( write(1, mesg, reads) == -1) {
      perror(E_WR);
      exit(2);
    }
  }
  exit(2);
}
