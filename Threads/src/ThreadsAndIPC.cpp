//============================================================================
// Name        : Threads.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : This program plays around with
//                a. signals, semaphores and alarms                            ( Signals.ccp )
//                b. GNU compiler synchronization built-ins for atomic access  ( builtIn.cpp )
//                c. p_threads using mutexes                                   ( ThreadHandling Simple.cpp)
//                d. IPC semaphores, shared memory ( XSI versions; formerly system V ) (IpcXsi.cpp )
//                d. p_thread IPC.  (PThreadIPC.cpp)
//                e. BoostIPC??  ( TBP.cpp )
//                g. Atomic Operations via C++11.
//============================================================================

#include <unistd.h>
#include <pthread.h>

#include <cassert>
#include <cstdio>

#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;

extern void builtIn(void);
extern void signalHandling(void);
extern void threadHandlingSimple(void);


int main()
{
    T_START
    printf( "Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__ );
    builtIn();
    signalHandling();
    threadHandlingSimple();
    T_END
    pthread_exit( NULL );
    return 0;
}
