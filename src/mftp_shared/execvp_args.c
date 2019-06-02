#include <stdlib.h>


//theses args are used to execute the ls and more processes in execvp.
const char *ls_cmd = "ls";
char *const ls_args[] = {"ls", "-l", ( void* )NULL};

const char *more_cmd = "more";
char *const more_args[] = {"more", "-20", ( void* )NULL};
