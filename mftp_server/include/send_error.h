#ifndef _GET_ERROR_H_
#define _GET_ERROR_H_

typedef enum {ERRNO, HERRNO} error_type;
char* send_error(int clientfd, error_type type, char* str);
int format(char*);

#endif
