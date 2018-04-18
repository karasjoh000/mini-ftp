SERVER=mftp_server
CFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include
mftpserve: mftpserve.o send_error.o configure_server.o zombiekiller.o control_commands.o
	gcc  configure_server.o mftpserve.o control_commands.o send_error.o zombiekiller.o -o mftpserve
mftpserve.o: mftpserve.c
	gcc $(CFLAGS) mftpserve.c
control_commands.o:
	gcc $(CFLAGS) $(SERVER)/control_commands.c
configure_server.o: $(SERVER)/configure_server.c
	gcc $(CFLAGS) $(SERVER)/configure_server.c
zombiekiller.o:
	gcc $(CFLAGS) $(SERVER)/zombiekiller.c
send_error.o:
	gcc $(CFLAGS) $(SERVER)/send_error.c
clean:
	rm *.o $(SERVER)/*.o
