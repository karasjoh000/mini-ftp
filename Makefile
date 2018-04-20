SERVER=mftp_server
CFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include -I.
mftpserve: mftpserve.o networkio.o debug.o send_error.o configure_server.o zombiekiller.o control_commands.o c
	gcc  configure_server.o mftpserve.o debug.o networkio.o control_commands.o send_error.o zombiekiller.o -o mftpserve
c: client.o debug.o send_error.o networkio.o
	gcc client.o debug.o networkio.o send_error.o -o c
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
networkio.o:
	gcc $(CFLAGS) networkio.c
debug.o:
	gcc $(CFLAGS) $(SERVER)/debug.c
client.o: client.c
	gcc $(CFLAGS) client.c
clean:
	rm *.o $(SERVER)/*.o
