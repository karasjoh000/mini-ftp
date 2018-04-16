SERVER=./mftp/server/
CLIENT=./mftp/client/
MFTPLIB=./mftp/
ISERVERDIR=$(SERVER)include/
ISERVER=-I$(SERVER)include/
ICLIENTDIR=$(CLIENT)include/
ICLIENT=-I$(CLIENT)
CFLAGS=-c -std=c99 -Wall -pedantic
mftpserve: mftpserve.o $(MFTPLIB)mftp.a
	gcc -L$(MFTPLIB) mftpserve.o -o mftpserve -lserver
mftpserve.o: mftpserve.c $(ISERVERDIR)server.h
	gcc $(CFLAGS) mftpserve.c $(ISERVERDIR)
$(MFTPLIB)mftp.a:
	cd $(MFTPLIB) && make mftp.a
clean:
	rm *.o ./mftp/*.o
