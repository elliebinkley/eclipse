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
//                e. threads using POSIX named/unamed semaphores ( Semaphore.cpp )
//                d. IPC semaphores, shared memory ( XSI versions; formerly system V ) ( todo )
//                e. BoostIPC                                                          ( todo )
//                f. C++11 IPC library                                                 ( todo )
//                g. Thread pool using select() etc..
//                h. fork processes and coordinate us9ng shared memory         ( SharedMemory.cpp )
//                i. reader/writer locks on a shared data structure.           ( todo )
//                j. memory barriers.                                          9 todo )
//
//                References:
//                1. http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap01.html
//=========================================================================================================

#include <unistd.h>
#include <pthread.h>

#include <cassert>
#include <cstdio>

#include <UtilitiesLib/inc/MyLogger.hpp>

using namespace std;

extern void atomicsTest( void );
extern void signalHandling( void );
extern void posixThreadHandlingWithPosixMutex( void );
extern void semaphores( void );
extern void threadHandlingwithMqueue();
extern void sharedMemory();
extern void conditionVariables();
extern void threadBarrier();

int main()
{
    T_START;

//    atomicsTest();
//    signalHandling();
//    posixThreadHandlingWithPosixMutex();
    threadHandlingwithMqueue();   // TODO assert
//    semaphores();                 // TODO seg fault[p

    sharedMemory();
    conditionVariables();

    threadBarrier();

    T_END;
    pthread_exit( NULL );
    return 0;
}
