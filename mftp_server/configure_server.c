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
#include <configure_server.h>

extern pthread_cond_t workerReleaser;  // for the thread that kills off
extern pthread_mutex_t m_workerReleaser; 				       // zombie processes.
extern pthread_t *releaserThread;      // thread that kills zombie processes.

extern int connectfd;

// SIGINT handler, releases mem and cancels second thread.
void shutdownServer(int code) {
	close(connectfd);
	printf("\nshutting down server\n");
	pthread_cancel(*releaserThread);
	free(releaserThread);
	exit(0);
}

void killZombies() {
	int stat;
	while (waitpid(-1, &stat, WNOHANG));
	return;
}

// releaserThread waiting function. When signaled, executes releaseWorkers().
void releaser(void* p) {
	while( 1) {
		pthread_mutex_lock(&m_workerReleaser);
		pthread_cond_wait(&workerReleaser, &m_workerReleaser);
		killZombies();
		pthread_mutex_unlock(&m_workerReleaser);
	}

}

// Forked process exectues this function, sends date/time to client then exits.
void serveClient(int netfd) {
	char buffer[40];
	time_t current = time(NULL);
	strcpy(buffer, ctime(&current));
	write(netfd, buffer, strlen(buffer) + 1); //+1 for null terminator.
	close(netfd);
	exit(0);
}

// configures server address (Port, family, and address).
void setServerAddress(struct sockaddr_in* servAddr) {
	memset(servAddr, 0, sizeof(*servAddr));
	servAddr->sin_family = AF_INET;
	servAddr->sin_port = htons(PORT);
	servAddr->sin_addr.s_addr = htonl(INADDR_ANY);

	return;
}

// when name is configured, it needs to be binded to the socket.
// listedfd -> socket fd
// servAddr -> struct that containes address configs.
void bindNameToSocket(int listenfd, struct sockaddr_in* servAddr) {
	int on = 1;
	// allow reuse of socket without waiting 3 min before using again.
	if ( setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) < 0)
		errx( 1, "error on sockopt: %s", strerror(errno));
	if ( bind( listenfd, /*assigns a name to an unnamed socket*/
		(struct sockaddr *) servAddr, sizeof(*servAddr)) < 0) {
			perror("bind");
			exit(1);
	}

}
