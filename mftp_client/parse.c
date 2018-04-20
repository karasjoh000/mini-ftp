#include <mftp.h>
#include <stdlib.h>
#include <string.h>
#include <parse.h>
#include <stdio.h>

char* readline() {
	int reads;
	char buffer[512];
	return (char*)NULL;
}

bool checkargs(char* line, char* split, int cmd) {
  char *path = NULL;
  //for commands with paths, make sure they have a path.
  //and that path is not to big.
  if( !(cmd ^ CD && cmd ^ RCD && cmd ^ GET
                 && cmd ^ PUT && cmd ^ SHOW ) ) {
    if(!(path = strtok(line, split))) {
      printf("Please provide a path\n");
      return false;
    }
    if (strlen(path) > CTRL_MSG_SIZE - 1) {
      printf("Path cannot fit to buffer");
      return false;
    }
  }
  if(!strtok(line, split)) {
    printf("To many arguments");
    return false;
  }
  line = path;
  return true;
}

int hash(char *str) {
  if(strcmp( str, "ls" )  == 0 ) return LS;
  if(strcmp( str, "rcd" ) == 0 ) return RCD;
  if(strcmp( str, "rls" ) == 0 ) return RLS;
  if(strcmp( str, "cd" )  == 0 ) return CD;
  if(strcmp( str, "get" ) == 0 ) return GET;
  if(strcmp(str, "put" )  == 0 ) return PUT;
  if(strcmp(str, "show" ) == 0 ) return SHOW;
  if(strcmp(str, "exit" ) == 0 ) return EXIT;
  return -1;
}
