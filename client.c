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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <err.h>
#include <errno.h>
#include <parse.h>
#include <stdbool.h>
#include <control_commands.h>
#include <connect.h>

#define CTRL_MSG_SIZE   512
#define CMD_SIZE        20

void program_loop(int controlfd, char* host) {
  char cmdbuffer[CMD_SIZE];
	char *split = " ";
	while(true) {
		printf("[mftp]:");
		fgets(cmdbuffer, CMD_SIZE, stdin);
    if (DEBUG) printf("read %s", cmdbuffer);
		switch( hash( strtok( cmdbuffer, split ) ) ) {
			case RCD:
        if(checkargs(cmdbuffer, split, RCD))
          rcd(controlfd, cmdbuffer);
        break;
      case GET:
        if(checkargs(cmdbuffer, split, GET))
          get(controlfd, cmdbuffer, host);
        break;
      case PUT:
        if(checkargs(cmdbuffer, split, PUT))
          put(controlfd, cmdbuffer, host);
        break;
			default:
        printf("Invalid command\n");
				break;
		}

	}


}

int main (int argc, char** argv) {

	if( argc == 1 )
		errx(0, "please provide a hostname in arguments");
  debugprint("in client");
  int controlfd = create_connection(argv[1], PORT);
  debugprint("got controlfd client");
  if (controlfd == -1) return 0;
  program_loop(controlfd, argv[1]);
  return 0;

}
