#include <stdio.h>
#include <mftp.h>

void debugprint( char* str ) {
  if ( DEBUG ) {
    printf( "%s\n", str );
  }
}
