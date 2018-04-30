#include <pthread.h>
#include <sys/wait.h>
#include <zombiekiller.h>
#include <mftp.h>

void killZombies() {
  int stat;
  while ( waitpid( -1, &stat, WNOHANG ) ); // kills all zombie processes until
                                           // NULL (no more left).
  return;
}


// releaserThread waiting function. When signaled, executes releaseWorkers().
void releaser( void* p ) {
  while( 1 ) {
    pthread_mutex_lock( &m_workerReleaser );
    pthread_cond_wait( &workerReleaser, &m_workerReleaser ); //waits to get signaled
    debugprint( "zombie killer signaled" );
    killZombies();
    pthread_mutex_unlock( &m_workerReleaser );
  }

}
