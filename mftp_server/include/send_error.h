#ifndef _SEND_ERROR_H_
#define _SEND_ERROR_H_

typedef enum {ERRNO, HERRNO} error_type;

void send_error(int clientfd, error_type type, char* str);
int error_format(char*);

#endif
