s: writetoclient.c server.c configure_server.c c
	gcc -I../../include/ server.c configure_server.c writetoclient.c -o s
c: client.c
	gcc client.c -o c
clean: 
	rm c s
