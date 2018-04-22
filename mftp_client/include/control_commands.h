#ifdef _CONTROL_COMMANDS_H_
#define _CONTROL_COMMANDS_H_

#define ERROR [ERROR]:

typedef enum {ERRNO, HERRNO, CUST} error_type;

void rcd(int fd, char* path);
int createdatac(int controlfd, char* host);
void get(int controlfd, char* path, char* host);
void put(int controlfd, char* path, char* host);
void print_error(error_type type, char* str);
bool isError(char* response);

#endif
