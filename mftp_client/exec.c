#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execvp_args.h>
#include <unistd.h>
#include <sys/wait.h>

const char *ls_cmd;
const char *ls_args[]; //= {"-20", NULL};

const char *more_cmd;
const char *more_args[]; //= {"-l", NULL};

void more();
void ls();

int main () {
	if ( fork () ) {
    int stat; wait(&stat);
	}
	else {
    more();
	}
  return 0;
}

void more() {

    int fd[2];
    pipe( fd );

    int status;
    if ( fork () ) {

		close( fd[1] );

		wait( &status );
		close( 0 );
		dup( fd[0] );
		close( fd[0] );

		execvp( more_cmd, more_args );

		exit ( 1 );
	}
	else {
		close( fd[0] );
		close( 1 );
		dup( fd[1] );
		close( fd[1] );
		ls();
		exit( 1 );
  }
}

void ls () {
  execvp( ls_cmd, ls_args );
}
