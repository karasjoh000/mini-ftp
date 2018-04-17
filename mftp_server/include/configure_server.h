#ifndef _CONFIGURE_SERVER_H_
#define _CONFIGURE_SERVER_H_

void shutdownServer(int code);
void killZombies();
void releaser(void* p);
void serveClient(int netfd);
void setServerAddress(struct sockaddr_in* servAddr, int port);
void bindNameToSocket(int listenfd, struct sockaddr_in* servAddr);
int get_port( int socketfd);

#endif
