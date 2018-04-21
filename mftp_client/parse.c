#include <mftp.h>
#include <stdlib.h>
#include <string.h>
#include <parse.h>
#include <stdio.h>


bool checkargs( char* arg1, char* arg2, char* path, int cmd) {

  debugprint("checking args");
  //strtok_r(NULL, split, context); //get rid of the first cmd.
  //for commands with paths, make sure they have a path.
  //and that path is not to big.
  if( !(cmd ^ CD && cmd ^ RCD && cmd ^ GET
                 && cmd ^ PUT && cmd ^ SHOW ) ) {
    if(!(arg1)) {
      printf("Error: Please provide a path\n");
      return false;
    }
    if (strlen(arg1) > CTRL_MSG_SIZE - 1) {
      printf("Error: Path cannot fit to buffer");
      return false;
    } else strcpy(path, arg1);
  }
  else if (!(cmd ^ EXIT && cmd ^ LS && cmd ^ RLS)) {
    if (arg1) {
      printf("Error: To many arguments");
      return false;
    } else return true;
  }
  if (arg2) {
    printf("To many arguements");
  }
  debugprint("changing new line to null");
  if (path[strlen(path)-1] == '\n') path[strlen(path)-1] = '\0';
  return true;
}

int hash(char *str) {
  if (DEBUG) printf("hashing %s\n", str);
  if(strcmp( str, "ls" )  == 0 ) return LS;
  if(strcmp( str, "rcd" ) == 0 ) return RCD;
  if(strcmp( str, "rls" ) == 0 ) return RLS;
  if(strcmp( str, "cd" )  == 0 ) {
    debugprint("returning cd code");
    return CD;
  }
  if(strcmp( str, "get" ) == 0 ) return GET;
  if(strcmp(str, "put" )  == 0 ) return PUT;
  if(strcmp(str, "show" ) == 0 ) return SHOW;
  if(strcmp(str, "exit" ) == 0 ) return EXIT;
  return -1;
}
