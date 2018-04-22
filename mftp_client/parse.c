#include <mftp.h>
#include <stdlib.h>
#include <string.h>
#include <parse.h>
#include <stdio.h>


bool checkargs(char *cmdline, int cmd) {
  char* ptr;
  switch(cmd) {
    case  CD:
    case RCD:
    case GET:
    case PUT:
      if( !( ptr = strtok(NULL, SPLIT) ) ) {
        printf("[ERROR]: Please provide a path\n");
        return false;
      }
      if (strtok(NULL, SPLIT)) {
        printf("[ERROR]: To many arguments\n");
        return false;
      }
      int plen;
      if ( (plen = strlen(ptr) ) > CTRL_MSG_SIZE - 1 ) {
        printf("[ERROR]: Path to big.");
        return false;
      }
      char temp[plen];
      strcpy(temp, ptr); strcpy(cmdline, temp);
      cmdline[plen - 1] = '\0'; //get rid of the new line.
      return true;
  }
  return false;
}



int hash(char *str) {
  if (DEBUG) printf("hashing %s\n", str);
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
