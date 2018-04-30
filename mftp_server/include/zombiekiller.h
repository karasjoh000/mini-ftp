#ifndef ZOMBIEKILLER_H
#define ZOMBIEKILLER_H

#include <pthread.h>


extern pthread_cond_t workerReleaser;     // for the thread that kills off
extern pthread_mutex_t m_workerReleaser;  // zombie processes.
extern pthread_t *releaserThread;      // thread that kills zombie processes.

/*void releaser( void* p ) : This is where the thread that kills zombie
		processes is sent. It waits until workerReleaser pthread_cond_t
		is signaled. When signaled it executes killZombies(). */
void releaser( void* p );

/*void killZombies() : Kills off zombie processes. */
void killZombies();


#endif
