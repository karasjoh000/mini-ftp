#ifndef ERRORS_H
#define ERRORS_H

      /* PRINT SUCCESS OR NOT ? */
#define SUCCESS    1

      /* ERROR STATMENTS USED ON CLIENT SIDE */
      /* SORRY, NO OPTION FOR THIS ONE */
#define E_BIG     "[ERROR]: Path to big.\n"
#define E_PATH    "[ERROR]: Please provide a path\n"
#define E_MANY    "[ERROR]: To many arguments\n"
#define E_CHDIR   "[ERROR]: Error changing directory"
#define E_CTRL    "[ERROR]: Error sending command to controller connection"
#define E_SERV    "[ERROR]: from server: %s\n"
#define E_OPEN    "[ERROR]: Cannot open file, check your permissions"
#define E_DATAC   "[ERROR]: Failed to create data connection with server\n"
#define E_ACK     "[ERROR]: Failed to read acknowledgement from server\n"
#define E_WR      "[ERROR]: write to file failed"
#define E_RD      "[ERROR]: read from file failed"
#define E_LS      "[ERROR]: failed to execute ls"
#define E_MORE    "[ERROR]: failed to execute more"
#define FATAL     "[FATAL ERROR]: Connection broken, exiting."


#endif