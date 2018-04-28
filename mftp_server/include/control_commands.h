#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include <stdbool.h>

typedef struct dataconn {
  int fd;
  int port;
  int io_fd;
} DATACON;

typedef enum {ERRNO, HERRNO, CUST} error_type;

bool changedir( char* path );
void create_data_connection( int controlfd, DATACON* info );
void get( int controlfd, int datafd, char* path );
void put( int controlfd, int datafd, char* path );
void send_error( int clientfd, error_type type, char* str );
int error_format( char* );
void send_ack( int controlfd, char* str );

#endif
