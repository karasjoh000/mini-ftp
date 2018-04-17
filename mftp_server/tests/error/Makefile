
CFLAGS=-I../../include/ -std=c99 -Wall -pedantic
s: server.c childserver.c configure_server.c send_error.c changedir.c debug.c c
	gcc $(CFLAGS) server.c childserver.c configure_server.c send_error.c changedir.c debug.c -o s
c: client.c debug.c send_error.c
	gcc $(CFLAGS) debug.c client.c send_error.c -o c
clean:
	rm c s
