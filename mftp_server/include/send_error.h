#ifndef _SEND_ERROR_H_
#define _SEND_ERROR_H_

typedef enum {ERRNO, HERRNO, CUST} error_type; //ernno, hernno, or custom message

void send_error(int clientfd, error_type type, char* str);
int error_format(char*);

#endif
