#ifndef PARSE_H
#define PARSE_H

#define CD       0 //update the hash function if this is changed.
#define GET      1
#define PUT      2
#define LS       3
#define SHOW     4
#define RCD      5
#define RLS      6
#define EXIT     7

#define SPLIT    " "

/*int hash( char* ) : hashes the string to a command code that are specified
			above. Hashes -1 if no code found for str. */
int hash( char* );

/*bool checkargs( char *cmdline, int cmd ) : given a command code checks if
		cmdline has the correct args. cmline was used by strtok before
		this function is called. */
bool checkargs( char *cmdline, int cmd );

/*bool getpath( char* path, char* cmdline ) : puts the path to cmdline*/
bool getpath( char* path, char* cmdline );

/*void removewhitespace(char* str) : removes leading and trailing whitespaces
 		in the user input. */
void removewhitespace(char* str);

#endif
