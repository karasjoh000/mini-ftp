#include <pthread.h>

void releaser(void* p);
void killZombies();

extern pthread_cond_t workerReleaser;  // for the thread that kills off
extern pthread_mutex_t m_workerReleaser; 				       // zombie processes.
extern pthread_t *releaserThread;      // thread that kills zombie processes.
