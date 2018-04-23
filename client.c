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

void program_loop(int controlfd, char* host) {
  char cmdbuffer[CMD_SIZE];
	while(true) {
		printf("[mftp]:");
		fgets(cmdbuffer, CMD_SIZE, stdin);
    if (DEBUG) printf("read %s", cmdbuffer);
		switch( hash( strtok( cmdbuffer, SPLIT) ) ) {
			case RCD:
        if(checkargs(cmdbuffer, RCD))
          rcd(controlfd, cmdbuffer);
        break;
      case GET:
        if(checkargs( cmdbuffer, GET))
          get(controlfd, cmdbuffer, host);
        break;
      case PUT:
        if(checkargs( cmdbuffer, PUT))
          put(controlfd, cmdbuffer, host);
        break;
      case CD:
        if (checkargs( cmdbuffer, CD))
          cd(cmdbuffer);
        break;
      case LS:
        if (checkargs( cmdbuffer, LS))
          printcontents(controlfd, PRINTLS, NULL, host);
        break;
      case RLS:
        if (checkargs( cmdbuffer, RLS))
          printcontents(controlfd, PRINTRLS, NULL, host);
        break;
			default:
        printf("Invalid command\n");
				break;
		}
	}
}

int main (int argc, char** argv) {

  debugprint("in client");
	if( argc == 1 )
		errx(0, "please provide a hostname in arguments");

  if(DEBUG) printf("Attempting to connect to port: %d\n", PORT);

  int controlfd = create_connection(argv[1], PORT);
  if (controlfd == -1) return 0;

  debugprint("got controlfd client");

  program_loop(controlfd, argv[1]);

  return 0;

}
