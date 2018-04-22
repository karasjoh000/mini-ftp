OBJ=./objects
SERVER=mftp_server
CLIENT=mftp_client
SHARED=mftp
SCFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include -I$(SHARED)/include
CCFLAGS=-c -std=c99 -Wall -pedantic -I$(CLIENT)/include -I$(SHARED)/include
mftpserve: $(OBJ) $(OBJ)/mftpserve.o $(OBJ)/networkio.o $(OBJ)/debug.o $(OBJ)/configure_server.o $(OBJ)/zombiekiller.o $(OBJ)/control_commands.o c
	gcc  $(OBJ)/mftpserve.o $(OBJ)/networkio.o $(OBJ)/debug.o $(OBJ)/configure_server.o $(OBJ)/zombiekiller.o $(OBJ)/control_commands.o -o mftpserve
c: $(OBJ) $(OBJ)/client.o $(OBJ)/debug.o $(OBJ)/connect.o $(OBJ)/networkio.o $(OBJ)/c_control_commands.o $(OBJ)/parse.o
	gcc $(OBJ)/client.o $(OBJ)/debug.o $(OBJ)/connect.o $(OBJ)/networkio.o $(OBJ)/c_control_commands.o $(OBJ)/parse.o -o c
$(OBJ)/mftpserve.o: mftpserve.c
	gcc $(SCFLAGS) mftpserve.c -o $(OBJ)/mftpserve.o
$(OBJ)/control_commands.o:
	gcc $(SCFLAGS) $(SERVER)/control_commands.c -o $(OBJ)/control_commands.o
$(OBJ)/configure_server.o: $(SERVER)/configure_server.c
	gcc $(SCFLAGS) $(SERVER)/configure_server.c -o $(OBJ)/configure_server.o
$(OBJ)/zombiekiller.o: $(SERVER)/zombiekiller.c
	gcc $(SCFLAGS) $(SERVER)/zombiekiller.c -o $(OBJ)/zombiekiller.o
$(OBJ)/networkio.o: $(SHARED)/networkio.c
	gcc $(CCFLAGS) $(SHARED)/networkio.c -o $(OBJ)/networkio.o
$(OBJ)/debug.o: $(SHARED)/debug.c
	gcc $(SCFLAGS) $(SHARED)/debug.c -o $(OBJ)/debug.o
$(OBJ)/connect.o: $(CLIENT)/connect.c
	gcc $(CCFLAGS) $(CLIENT)/connect.c -o $(OBJ)/connect.o
$(OBJ)/client.o: client.c
	gcc $(CCFLAGS) client.c -o $(OBJ)/client.o
$(OBJ)/parse.o: $(CLIENT)/parse.c
	gcc $(CCFLAGS) $(CLIENT)/parse.c -o $(OBJ)/parse.o
$(OBJ)/c_control_commands.o: $(CLIENT)/control_commands.c $(OBJ)
	gcc $(CCFLAGS) $(CLIENT)/control_commands.c -o $(OBJ)/c_control_commands.o
$(OBJ):
	mkdir $(OBJ)
clean:
	rm -rf $(OBJ) mftpserve c
