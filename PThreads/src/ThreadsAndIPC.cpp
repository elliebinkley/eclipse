//============================================================================
// Name        : Threads.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : This program plays around with
//                a. signals, semaphores and alarms                            ( Signals.ccp )
//                b. Atomic operations using std:atomic( C++11 ) and GNU compiler
//                                synchronization built-ins for atomic access  ( Atomics.cpp )
//                c. p_threads using mutexes                                   ( ThreadHandlingSimple.cpp)
//                d. threads and processes using POSIX message queues          ( MQueue.cpp )
//                e. threads and processes using POSIX named/unamed semaphores ( Semaphore.cpp )
//                d. IPC semaphores, shared memory ( XSI versions; formerly system V ) ( todo )
//                e. BoostIPC                                                          ( todo )
//                f. C++11 IPC library                                                 ( todo )
//                g. Thread pool using select() etc..
//                h. shared memory
//                i. reader/writer locks on a shared data structure.
//                j. memory barriers.
//
//                References:
//                1. http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap01.html
//=========================================================================================================

#include <unistd.h>
#include <pthread.h>


#include <cassert>
#include <cstdio>

#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;

extern void atomicsTest(void);
extern void signalHandling(void);
extern void posixThreadHandlingWithPosixMutex(void);
extern void ipcXsi(void);
extern void threadHandlingwithMqueue();
extern void sharedMemory();

int main()
{
    T_START;
/*
    atomicsTest();
    signalHandling();
    posixThreadHandlingWithPosixMutex();
    threadHandlingwithMqueue();
    ipcXsi();
*/
    sharedMemory();


    T_END;
    pthread_exit( NULL );
    return 0;
}
