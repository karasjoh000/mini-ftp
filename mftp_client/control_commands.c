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

//see mftp/execvp_args.c for the arguments.
const char *ls_cmd;
char *const ls_args[];

const char *more_cmd;
char *const more_args[];


bool isError( char* response ) {
  if( response[0] == 'E' ) { // if message is an error, return true.
    printf( E_SERV, &response[1] );
    return true;
  }
  return false;
}

void quit( int controlfd ) {
  char mesg[CTRL_MSG_SIZE];
  strcpy( mesg, "Q\n" ); //send quit to server and wait for acknowlegdment.
  if ( write( controlfd, mesg, strlen( mesg ) ) == -1 ) quitwitherror();
  if ( !read_ack( controlfd ) ) quitwitherror();
  exit(0);
}


void rcd( int controlfd, char* path ) {
  debugprint( "in rcd" );
  if ( DEBUG ) printf( "remote cd path:%s\n", path );
  char mesg[CTRL_MSG_SIZE];
  sprintf( mesg, "C%s\n", path ); // change dir on server and read acknowlegdment.
  if( DEBUG ) printf( "Sending to controller: %s\n", mesg );
  if ( write( controlfd, mesg, strlen( mesg ) ) == -1 ) quitwitherror();
  read_ack(controlfd);
}



bool cd( char* path ) {
  debugprint( "in change dir routine" );
  if ( chdir( path ) == -1 ) { //change dir, return false if error.
    perror( E_CHDIR );
    return false;
  }
  return true;
}




void get( int controlfd, char* path, char* host ) {
  char mesg[CTRL_MSG_SIZE];
  int datafd;

  //try to open the file in path, return if error.
  if( DEBUG ) printf( "creating %s...\n", path );
  int filefd = open ( getname( path ), O_RDWR | O_CREAT, 0755 );
  if ( filefd == -1 ) {
    perror( E_OPEN );
    return;
  }

  // create a new data connection. Return if error.
  debugprint( "creating data connection..." );
  if ( ( datafd = createdatac( controlfd, host ) ) == -1 ) {
    printf( E_DATAC );
    close(filefd);
    return;
  }
  debugprint( "[SUCCESS]: Data connection created" );

  //send the get command on path to server
  sprintf( mesg, "G%s\n", path );
  if ( DEBUG ) printf( "sending over controller: %s", mesg );
  if ( write( controlfd, mesg, strlen( mesg ) ) == -1 ) quitwitherror();

  //read acknowlegdment from server if error close fd's and return.
  if( !read_ack(controlfd) ) {
    close( datafd );
    close(filefd);
    return;
  }

  //copy contents on datafd to filefd.
  catchfile( datafd, filefd );
  close(datafd);
  close(filefd);
}





void put( int controlfd, char* path, char* host ) {
  char mesg[CTRL_MSG_SIZE];

  //try opening file return if error.
  int filefd = open ( path, O_RDONLY, 0 );
  if ( filefd == -1 ) {
    perror( E_OPEN );
    return;
  }

  //create a new data connection return if there is an error.
  int datafd = createdatac( controlfd, host );
  if ( datafd == -1 ) {
    close(filefd);
    printf( E_DATAC );
    return;
  }


  //send the put command to server.
  debugprint( "[SUCCESS]: Data connection created" );
  sprintf( mesg, "P%s\n", path );
  if( DEBUG ) printf( "[SENDING]: sending over controller: %s ", mesg );
  if ( write( controlfd, mesg, strlen( mesg ) ) == -1 ) quitwitherror();


  // read controlds for acknowlegdment. Return if error reading or Error from server.
  if( !read_ack(controlfd) ) {
    close( datafd );
    close(filefd);
    return;
  }

  //copy the contents of filefd to datafd.
  chuckfile( datafd, filefd );
  close(datafd);
  close(filefd);
  debugprint( "[SUCCESS]: File sent to server" );

}




int createdatac( int controlfd, char* host ) {
  char mesg[CTRL_MSG_SIZE];


  //send the new data connection to server.
  if ( write( controlfd, "D\n", 2 ) == -1 )  quitwitherror();

  //read the port # from control connection return if error while reading.
  if( !readcontroller( controlfd, &mesg, CTRL_MSG_SIZE ) ) {
    printf( E_ACK );
    return -1;
  }

  int port; //read port # from message return -1 if error parsing.
  if ( sscanf( mesg, "A%d", &port ) != 1 ) return -1;
  return create_connection( host, port ); //return data connection fd.

}


bool read_ack( int controlfd ) {
  char mesg[CTRL_MSG_SIZE];
  if( !readcontroller( controlfd, &mesg, CTRL_MSG_SIZE ) ) {
    printf( E_ACK );
    return false;
  }
  if( isError( mesg ) ) return false;
  return true;
}


void printcontents( int controlfd, print_type type, char* path, char* host ) {
  int datafd;
  char mesg[CTRL_MSG_SIZE];
  if ( type == PRINTSHOW || type == PRINTRLS ) {
    if ( ( datafd = createdatac( controlfd, host ) ) == -1 ) {
      printf( E_DATAC );
      return;
    }
    switch ( type ) {
    case PRINTRLS:
      strcpy( mesg, "L\n" );
      break;
    case PRINTSHOW:
      sprintf( mesg, "G%s\n", path );
      break;
    }
    if( write( controlfd, mesg, strlen( mesg ) ) == -1 ) {
      perror( FATAL );
      exit( 1 );
    }
    if( !read_ack( controlfd ) ) {
      printf( E_ACK );
      close( datafd );
      return;
    }

  }

  if ( fork() ) {
    int status;
    wait( &status );
    return;
  } else {
    debugprint( "In more process" );
    more( controlfd, datafd, type, path, host );
  }
}


void more( int controlfd, int datafd, print_type type, char *path, char *host ) {
  int fd[2];
  pipe( fd );

  if( fork() ) {
    close( fd[1] );
    int status;
    wait( &status );
    close( 0 );
    dup( fd[0] );
    close( fd[0] );
    execvp( more_cmd, more_args );
    exit ( 2 );
  } else {
    close( fd[0] );
    close( 1 );
    dup( fd[1] );
    close( fd[1] );
    switch( type ) {
    case PRINTLS:
      ls();
      break;
    case PRINTRLS:
    case PRINTSHOW:
      servprint( controlfd, datafd, host );
      break;
    default:
      exit(1);
    }
    exit(1);
  }
}


void ls() {
  execvp( ls_cmd, ls_args );
  printf( "Failed to execute ls\n" );
  exit( 2 );
}

void servprint( int controlfd, int datafd, char* host ) {
  debugprint( "in servprint" );
  char mesg[CTRL_MSG_SIZE];
  int reads;

  while( ( reads = read( datafd, mesg, CTRL_MSG_SIZE ) ) != 0 ) {
    if ( write( 1, mesg, reads ) == -1 ) {
      perror( E_WR );
      exit( 2 );
    }
  }
  exit( 2 );
}
