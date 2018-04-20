SERVER=mftp_server
SHARED=mftp
CFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include -I.
mftpserve: mftpserve.o networkio.o debug.o configure_server.o zombiekiller.o control_commands.o c
	gcc  configure_server.o mftpserve.o debug.o networkio.o control_commands.o zombiekiller.o -o mftpserve
c: client.o debug.o networkio.o
	gcc client.o debug.o networkio.o -o c
mftpserve.o: mftpserve.c
	gcc $(CFLAGS) mftpserve.c
control_commands.o:
	gcc $(CFLAGS) $(SERVER)/control_commands.c
configure_server.o: $(SERVER)/configure_server.c
	gcc $(CFLAGS) $(SERVER)/configure_server.c
zombiekiller.o:
	gcc $(CFLAGS) $(SERVER)/zombiekiller.c
networkio.o: $(SHARED)/networkio.c
	gcc $(CFLAGS) $(SHARED)/networkio.c
debug.o: $(SHARED)/debug.c
	gcc $(CFLAGS) $(SHARED)/debug.c
client.o: client.c
	gcc $(CFLAGS) client.c
clean:
	rm *.o $(SERVER)/*.o
