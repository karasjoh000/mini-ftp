SERVER=mftp_server
CFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include
mftpserve: mftpserve.o configure_server.o
	gcc  configure_server.o mftpserve.o -o mftpserve
mftpserve.o: mftpserve.c
	gcc $(CFLAGS) mftpserve.c
configure_server.o: $(SERVER)/configure_server.c
	gcc $(CFLAGS) $(SERVER)/configure_server.c
clean:
	rm *.o $(SERVER)/*.o
