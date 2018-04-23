#include <pthread.h>
#include <sys/wait.h>


pthread_cond_t workerReleaser;  // for the thread that kills off
pthread_mutex_t m_workerReleaser; 				       // zombie processes.
pthread_t *releaserThread;      // thread that kills zombie processes.


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
    debugprint("zombie killer signaled");
		killZombies();
		pthread_mutex_unlock(&m_workerReleaser);
	}

}
