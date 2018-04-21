SERVER=mftp_server
CLIENT=mftp_client
SHARED=mftp
SCFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include -I.
CCFLAGS=-c -std=c99 -Wall -pedantic -I$(CLIENT)/include -I.
mftpserve: mftpserve.o networkio.o debug.o configure_server.o zombiekiller.o control_commands.o c
	gcc  configure_server.o mftpserve.o debug.o networkio.o control_commands.o zombiekiller.o -o mftpserve
c: client.o debug.o connect.o networkio.o c_control_commands.o prase.o
	gcc client.o connect.o debug.o c_control_commands.o parse.o networkio.o -o c
mftpserve.o: mftpserve.c
	gcc $(SCFLAGS) mftpserve.c
control_commands.o:
	gcc $(SCFLAGS) $(SERVER)/control_commands.c
configure_server.o: $(SERVER)/configure_server.c
	gcc $(SCFLAGS) $(SERVER)/configure_server.c
zombiekiller.o: $(SERVER)/zombiekiller.c
	gcc $(SCFLAGS) $(SERVER)/zombiekiller.c
networkio.o: $(SHARED)/networkio.c
	gcc $(CCFLAGS) $(SHARED)/networkio.c
debug.o: $(SHARED)/debug.c
	gcc $(SCFLAGS) $(SHARED)/debug.c
connect.o: $(CLIENT)/connect.c
	gcc $(CCFLAGS) $(CLIENT)/connect.c
client.o: client.c
	gcc $(CCFLAGS) client.c
prase.o: $(CLIENT)/parse.c
	gcc $(CCFLAGS) $(CLIENT)/parse.c
c_control_commands.o: $(CLIENT)/control_commands.c
	gcc $(CCFLAGS) $(CLIENT)/control_commands.c -o c_control_commands.o
clean:
	rm *.o $(SERVER)/*.o
