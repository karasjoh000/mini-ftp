//void data_connection( int* pipefd );
//void notifyServer(char* stre, int length, int* fd );
typedef struct dataconn {
	int fd;
	int port;
} DATACON;

void create_data_connection(DATACON* info, int clientfd);
