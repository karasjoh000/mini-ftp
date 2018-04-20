/*
   John Karasev
   CS 360 Systems Programming
   WSUV Spring 2018
   -----------------------------------------------------
   Assignment #9:
   Using network sockets, make a server and client where client gets time
   from the server. This file is the implementation of the client side.
*/
#include <mftp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <err.h>
#include <errno.h>
#include <parse.h>
#include <control_commands.h>
#include <connect.h>

#define CTRL_MSG_SIZE   512
#define CMD_SIZE        4096

void program_loop(int controlfd, char* host) {
	char split = ' ';
	while(true) {
		printf("[mftp]:");
		char *line = readline();
		switch( hash( strtok( line, &split ) ) ) {
			case RCD:
        if(checkargs(line, &split, RCD))
          rcd(controlfd, line);
        break;
      case GET:
        if(checkargs(line, &split, GET))


			defualt:
				break;
		}

	}


}

int main (int argc, char** argv) {

	if( argc == 1 )
		errx(0, "please provide a hostname in arguments");

  int controlfd = create_connection(argv[1], PORT);
  if (controlfd == -1) return 0;
  program_loop(controlfd, argv[1]);
  return 0;

}
