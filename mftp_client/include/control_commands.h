#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include <stdbool.h>

typedef enum {ERRNO, HERRNO, CUST} error_type;
typedef enum {PRINTLS, PRINTRLS, PRINTSHOW} print_type;

#define CTRL_MSG_SIZE   512
#define CMD_SIZE        550

void rcd(int fd, char* path);
bool cd(char* path);
int createdatac(int controlfd, char* host);
void get(int controlfd, char* path, char* host);
void put(int controlfd, char* path, char* host);
void print_error(error_type type, char* str);
void printcontents(int controlfd, print_type type, char* path, char* host);
void more(int controlfd, print_type type, char *host);
bool isError(char* response);
void ls();
void show(int controlfd, char *host);
void rls(int controlfd, char *host);

#endif
