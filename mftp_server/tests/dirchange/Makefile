s: server.c childserver.c configure_server.c send_error.c changedir.c c
	gcc -I../../include/ server.c childserver.c configure_server.c send_error.c changedir.c -o s
c: client.c
	gcc -I../../include/ client.c send_error.c -o c
clean:
	rm c s
