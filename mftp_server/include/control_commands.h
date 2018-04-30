#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include <stdbool.h>

/* DATACON stores information about a new dataconn. If error then port is set to
   -1.  */
typedef struct dataconn {
  int fd;
  int port;
  int io_fd;
} DATACON;

/* Used for send_error subroutine to determine what type of error to send to client.
   Can be either errno, herrno, or custom type where a pointer to a error message
   string is provided. */
typedef enum {ERRNO, HERRNO, CUST} error_type;

/*  changedir(char*) :  Changes the dir for a child server. Returns true if
  			successfully changes dir false if error. */
bool changedir( char* path );

/* void create_data_connection( int controlfd, DATACON* info ) : creates data
		        connection and sets the the feilds of
		        DATACON struct. If error, sets port to -1
			and sends error to client. */
void create_data_connection( int controlfd, DATACON* info );

/* void get( int controlfd, int datafd, char* path ) :  given path to a file and
   			a datafd that was made through create_data_connection, opens the file on path
   			and sends it through the datafd. If error opening the file then returns
   			error to client. If error when trying to write to client then assumes that
   			connection is broken and quites the child server.   */
void get( int controlfd, int datafd, char* path );

/*void put( int controlfd, int datafd, char* path ) : given a path name, copies
          		contents on datafd to a file on current dir with the
			name found in path. */
void put( int controlfd, int datafd, char* path );


/*void send_error( int clientfd, error_type type, char* str ) : sends error to
			the client. If error_type is ERRNO then sends the
			message from strerror(errno) to client with E in begining
			and \n at end. Same for HERROR except uses
			hstrerror(h_errno). If CUST is provided then it prints
			the string provided in args. Concats E and \n to the
			strings before sending. */
void send_error( int clientfd, error_type type, char* str );

/*void send_ack( int controlfd, char* str ) : sends acknowledgment to client.
			If str is not NULL it sends '"A%s\n", str'
			otherwise '"A\n"'. */
void send_ack( int controlfd, char* str );

#endif
