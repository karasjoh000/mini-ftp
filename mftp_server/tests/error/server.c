#include <configure_server.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <err.h>
#include <errno.h>
#include <childserver.h>
#include <debug.h>

#define PORT 49999

pthread_cond_t workerReleaser;  // for the thread that kills off
pthread_mutex_t m_workerReleaser; 				       // zombie processes.
pthread_t *releaserThread;      // thread that kills zombie processes.

int connectfd;

extern void releaser(void*);
extern void shutdownServer(int);

int main () {
	// cancel threads and release memory then exit, see shutdownServer.
	signal( SIGINT, shutdownServer );
	// init list of pid's.
	// make a seperate thread that will waitpid WNOHANG on the list of pid's.
	releaserThread = ( pthread_t* ) malloc( sizeof( pthread_t ));
	pthread_create(releaserThread, NULL, ( void* ) releaser, NULL);

	// get a fd for socket
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) errx( 1, "error getting socket fd: %s", strerror(errno));

	struct sockaddr_in servAddr;

	setServerAddress(&servAddr, PORT); //set address, port, and add. family.
	bindNameToSocket(listenfd, &servAddr); // bind address to socket.

	// set queue limit for incoming connections
	if(listen(listenfd, 1) == -1)
		errx( 1, "error setting connections queue: %s", strerror(errno));


	unsigned int length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;



	while (1) {

		int fd[2];
		pipe(fd);
		close(fd[1]);

		struct hostent* hostEntry;

		pid_t pid;

		printf("listing on port %d\n", PORT);

		if ( (connectfd = accept( listenfd, (struct sockaddr *) &clientAddr,
		      &length)) == -1 )
			errx( 1, "error connecting: %s", strerror(errno));

		// get ip address of client.
		char clientip[INET_ADDRSTRLEN];
		if ( ( inet_ntop( AF_INET, &clientAddr.sin_addr,
		     clientip, INET_ADDRSTRLEN ) ) == NULL)
			errx( 1, "error connecting: %s", strerror(errno));

		// get hostname of client.
		hostEntry = gethostbyaddr(&(clientAddr.sin_addr),
			   sizeof(struct in_addr), AF_INET);

		if (!hostEntry)
			printf("accepted client [name: unknown][ip: %s]\n", clientip);
		printf("accepted client [name: %s][ip: %s]\n", hostEntry->h_name, clientip);

		// create new process to serve client.
		if ( !( pid = fork() ) )  childserver(fd);

		char controller_response[50];
		int mesg_length = read(fd[0], controller_response, 50);
		controller_response[mesg_length] = '\0';
		debugprint("received acknowlegement from childserver");
		//debugprint(controller_response);
		close(fd[0]);
		write(connectfd, controller_response, mesg_length);
		close(connectfd);

		printf("process %d is serving client\n", pid);

		// signal thread to kill zombie processes.
		pthread_cond_signal(&workerReleaser);

	}

	return 0;

}
