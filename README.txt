JOHN KARASEV
CS 360 SPRING 2018
FINAL PROJECT


Description:
 	See assignment pdf for details.


Program structure:

CS360_final
├── Makefile                    # makefile
├── README.txt                  # this file
├── mftp.c			# client main function
├── mftpserve.c                 # server main function
├── mftp_client
│   ├── connect.c               # functions to create connections with server
│   ├── control_commands.c      # functions for control connection
│   ├── parse.c                 # user input parse functions
│   └── include
│       ├── connect.h
│       ├── control_commands.h
│       └── parse.h
│  
├── mftp_server
│   ├── configure_server.c	# functions to configure server sockets
│   ├── control_commands.c      # functions for control connection.
│   ├── zombiekiller.c          # seperate thread that kills zombie processes
│   └── include
│       ├── configure_server.h
│       ├── control_commands.h
│       └── zombiekiller.h
│  
└── mftp_shared
    ├── debug.c                 # debug functions
    ├── execvp_args.c           # args for more and ls in execvp
    ├── networkio.c             # functions used for network io
    └── include
        ├── mftp.h
        └── responses.h         # error responses, mostly used on client side.
   


mftp_share : Common to both client and server.
mftp_server: Used in server only.
mftp_client: Used in client only.
