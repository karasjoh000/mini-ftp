#include <mftp.h>
#include <stdlib.h>
#include <string.h>
#include <parse.h>
#include <stdio.h>
#include <responses.h>
#include <stdbool.h>
#include <control_commands.h>
#include <ctype.h>

void removewhitespace(char* str) {
    int begofstr, endofstr;
    for (begofstr = 0; begofstr < strlen(str) && isspace(str[begofstr]); begofstr++);
    str = &str[begofstr];
    for (endofstr = strlen(str) - 1; endofstr >= 0 && isspace(str[endofstr]); endofstr--);
    str[endofstr + 1] = '\0';
}

bool getpath( char* path, char* cmdline ) {

  int plen = strlen( path );
  if ( ( plen = strlen( path ) ) > CTRL_MSG_SIZE - 1 ) {
    printf( E_BIG );
    return false;
  }
  char temp[plen];
  strcpy( temp, path ); //need to make a temp because ptr is pointing inside cmdline buffer
  strcpy( cmdline, temp );
  return true;
}

void stripchar( char* str, char ch ) {
  char *pch; //get rid of the last newline
  if( ( pch = strrchr( str, ch ) ) ) * pch = '\0';
  return;
}

bool checkargs( char *cmdline, int cmd ) {
  char* ptr;
  switch( cmd ) {
  case  CD:
  case RCD:
  case GET:
  case PUT:
  case SHOW:
    {
      if( !( ptr = strtok( NULL, SPLIT ) ) ) {
        printf( E_PATH );
        return false;
      }
      if ( strtok( NULL, SPLIT ) ) {
        printf( E_MANY );
        return false;
      }
      return getpath( ptr, cmdline );
      break;
    }
  case LS:
  case RLS:
  case EXIT:
    {
      if( ( ptr = strtok( NULL, SPLIT ) ) ) {
        printf( E_MANY );
        return false;
      }
      return true;
    }
  }
  return false;
}



int hash( char *str ) {
  char format[CMD_SIZE]; //need to make a copy to not screw strtok interator.
  removewhitespace( strcpy( format, str) );

  if ( DEBUG ) printf( "hashing %s\n", format );
  if( strcmp( format, "ls"   )  == 0 ) return LS;
  if( strcmp( format, "rcd"  )  == 0 ) return RCD;
  if( strcmp( format, "rls"  )  == 0 ) return RLS;
  if( strcmp( format, "cd"   )  == 0 ) return CD;
  if( strcmp( format, "get"  )  == 0 ) return GET;
  if( strcmp( format,  "put"  ) == 0 ) return PUT;
  if( strcmp( format,  "show" ) == 0 ) return SHOW;
  if( strcmp( format,  "exit" ) == 0 ) return EXIT;
  return -1;
}
