SERVER=./server/
CFLAGS=-c -std=c99 -Wall -pedantic -I$(SERVER)/include

mftp.a: server.o
	ar cru mftp.a server.o
	ranlib mftp.a
server.o:
	gcc $(CFLAGS) $(SERVER)server.c
clean:
	rm *.o
