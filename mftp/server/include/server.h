void shutdownServer(int code);
void killZombies();
void releaser(void* p);
void serveClient(int netfd);
void setServerAddress(struct sockaddr_in* servAddr);
void bindNameToSocket(int listenfd, struct sockaddr_in* servAddr);
#define PORT 49999
