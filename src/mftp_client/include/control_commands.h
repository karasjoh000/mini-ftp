#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include <stdbool.h>


// specifies what to print to stdout for the more process. See printcontents.
typedef enum {PRINTLS, PRINTRLS, PRINTSHOW} print_type;

#define CMD_SIZE        550 //define how big the commandline arguements can be


/*void quit( int controlfd ) : Notifies server to quit and waits for ack. If no
			acknowledgment, exit with error. */
void quit( int controlfd );

/*bool isError( char* response ) : parses control connection response. If error,
			prints it to stdout and returns false, otherwise returns
			true. */
bool isError( char* response );

/*bool read_ack( int controlfd ) : reads acknowledgment from control connection.
			If an error message is recieved, error message is printed
			to stdout and false is returned. If an acknowledgment is
			received true is returned. If failed to read from control
			connection then error is printed to stdout and false is
			returned. */
bool read_ack( int controlfd );

/* void rcd( int fd, char* path ) : sends server command to change dir to path.
			Uses read_ack to read reponse from server. */
void rcd( int fd, char* path );

/*changes directory on the server side. If success true is returned
			otherwise false. */
bool cd( char* path );

/*int createdatac( int controlfd, char* host ) : notifies server to create a new
			data connection. If success returns socket fd for the
			data connection otherwise -1 if error. */
int createdatac( int controlfd, char* host );

/*void get( int controlfd, char* path, char* host ) : gets the contents of
			file on path in server. First creates data connection
			using createdatac. Then if createdatac is successful,
			the get command is sent to server.
			Then waits for acknowledgment before reading from data
			connection. */
void get( int controlfd, char* path, char* host );

/*void put( int controlfd, char* path, char* host ) : puts the contents of
			file (path) on client side to server side.
			prints error to stdout if unsuccessful. */
void put( int controlfd, char* path, char* host );

/*void printcontents( int controlfd, print_type type, char* path, char* host ) :
			Uses more command to either print the contents of
			rls, show, or ls which are specified in print_type
			enum. */
void printcontents( int controlfd, print_type type, char* path, char* host );

/*void more( int controlfd, int datafd, print_type type, char *path, char *host ) :
			creates a pipe to print contents of child process to
			stdout for the more process. Uses ls() in child process when
			PRINTLS is given otherwise executes servprint which reads
			data from server. */
void more( int controlfd, int datafd, print_type type, char *path, char *host );

/*void ls() : executes the ls command with the arguements provided in
			mftp/execvp_args.c */
void ls();

/*void servprint( int controlfd, int datafd, char* host ) : Reads contents on
			datafd and prints them to stdout. This will be redirected
			to the more process. */
void servprint( int controlfd, int datafd);

#endif
