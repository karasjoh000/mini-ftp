#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include <stdbool.h>

typedef enum {ERRNO, HERRNO, CUST} error_type;
typedef enum {PRINTLS, PRINTRLS, PRINTSHOW} print_type;

void rcd(int fd, char* path);
bool cd(char* path);
int createdatac(int controlfd, char* host);
void get(int controlfd, char* path, char* host);
void put(int controlfd, char* path, char* host);
void print_error(error_type type, char* str);
void printcontents(char* path, print_type type, int controlfd);
void more20(int controlfd, int *morepipe, print_type type);
bool isError(char* response);
void ls(int *prin_con_pipe);
void show(int controlfd, int *prin_con_pipe);
void rls(int controlfd, int *prin_con_pipe);

#endif
