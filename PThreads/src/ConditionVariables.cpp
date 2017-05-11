/*
 *  ConditionVaribles.cpp
 *
 *  Created on: May 9, 2017
 *      Author: Burley
 *      Description:  Sample program for thread barriers and conditional variables.
 *
 *      1.Create shared memory ( shm_open()  ) with a named location.  Produced a file under /def/shm ..
 *      2.Map shared memory mmap().
 *      3.create a X processes via fork() and wait via waitpid()
 *        Note that processes created using fork() do not need to use shm_open(),
 *        since forked processes inherit the memory mapped IO file descriptors.
 *        ( But independent, unrelated processes need to use shm_open() to get the file descriptor. )
 *      4. The forked processes use a conditional mutex to wait till the last process has been created before they
 *         start doing any work.
 *         The conditional mutex is in the shared memory area.
 *         Note that mutexes and conditional variables cannot be PTHREAD_PROCESS_SHARED in cygwin; a
 *         CYWGIN limitation apparently casued by windows threading.  This causes the pthread_cond_timedwait()
 *         to time-out rather than awaken via pthread_cond_broadcast().
 *      5. Each forked access accesses shared memory via mmap() with the inherited file descriptor.
 *      6.
 *      7. Once all forked processes have finished, the manager/main process will check if the sums match.
 *
 *      References:
 *      1. shm_open,  shm_unlink  -  create/open  or  unlink POSIX shared memory
 *      2. mmap, munmap, msync() - map or unmap files or sync devices into memory
 *      3. pthread_cond_waittimed(), pthread_cond_broadcast()
 *      4. pthread_mutex_lock(), pthread_mutex_unlock()
 *      See https://www3.physnet.uni-hamburg.de/physnet/Tru64-Unix/HTML/APS33DTE/DOCU_004.HTM
 */

#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>       // sem_t
#include <fcntl.h>           // O_CREAT

#include <cstdio>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <ctime>
#include "time.h"
#include <fcntl.h>           /* For O_* constants */

#include <string>
#include <sstream>
#include <iostream>
#include <atomic>

#include "SharedLibrary/inc/MyLogger.hpp"
#include "../inc/PThreadUtilities.hpp"

static const unsigned int NUM_PROC = 90; // 256 is the max number of processes allowed; see ulimit -a
static const char* MEMORY_NAME = "/memory_name";  // name of shared memory under /dev/shm durectory
static unsigned int BUFSIZE = 100;                // name of sh

static bool Debug = false;
static int fd;

struct regionCondShm
{ /* Defines "structure" of shared memory */
    char testName[100];
    volatile std::atomic<unsigned int> numProcsInitialized;
    volatile unsigned int numTimeoutErrors = 0;  // number of times
    NUMERROR_TYPE numErrors;       // general error counter for this application.
    pthread_mutex_t count_mutex;                 // condition variable mutex
    pthread_cond_t all_processes_created;        // condition variable.
};

// functions in this file
static int doChildActions( unsigned int i );
static int openShm( bool create );
static regionCondShm* mapSharedMem( int fd );
static void createProcesses( pid_t* pidTble );
static void waitForAllProcessesToInit( regionCondShm* const rptr );
static void waitForProcess( pid_t* pidTble );
static void coordinateTests( regionCondShm* const rptr );
static void initSharedMem( regionCondShm* const rptr );
static void flushToDisk( regionCondShm* const rptr );
static void cleanupSharedMem( regionCondShm** const rptr );
static void printSharedMem( regionCondShm* const rptr );

using namespace std;

void conditionVariables()
{
    T_START;
    pid_t pidTble[NUM_PROC];
    regionCondShm* rptr = nullptr;
    try
    {
        fd = openShm( true );
        rptr = mapSharedMem( fd );
        initSharedMem( rptr );
        createProcesses( pidTble );

        coordinateTests( rptr );

        waitForProcess( pidTble );

        flushToDisk( rptr );
        printSharedMem( rptr );
        cleanupSharedMem( &rptr );

        // get rid of shared memory
        if( shm_unlink(MEMORY_NAME) )
        {
            //   throwARunTimeError("shm_unlink()", true, nullptr, __LINE__,0);

            throwARunTimeError("shm_unlink()", true, nullptr, __LINE__);

        }

        T_LOG( " SUCCESS!! ");
    }
    catch ( std::exception &ex)
    {
        T_LOG("caught exception");
        cleanupSharedMem( &rptr );
        shm_unlink( MEMORY_NAME );
        T_LOG( ex.what() );
    }

    T_END;
}

// called by main process to open shared memory
int openShm( bool create )
{
    T_START;
    mode_t mode = 0x666;
    int oflag = create ? (O_CREAT | O_EXCL | O_RDWR) : ( O_RDWR );
    int fd;
    if( ( fd = shm_open(MEMORY_NAME, oflag, mode )) == -1 )
    {
        throwARunTimeError("shm_open()", true, nullptr, __LINE__ );
    }

    // truncate  the file to the length of the region
    if (ftruncate(fd, sizeof(struct regionCondShm)) == -1)
    {
        throwARunTimeError("ftruncate()", true, nullptr, __LINE__ );
    }

    T_END;
    return fd;
}

int doChildActions( unsigned int procNum )
{
    if( Debug ) T_START;
    try
    {
        // print out pid and i number
        if( Debug )
        {
            stringstream ss;
            ss << "procNum =" << procNum << " pid=" << getpid() << " &fd=" << &fd;
            T_LOG( ss.str().c_str() );
        }

        // memory map the shared memory
        regionCondShm* const rptr = mapSharedMem( fd );

        waitForAllProcessesToInit( rptr );// use condition variables to synchronize

        if( Debug )
        {
            printSharedMem( rptr );
        }

    }
    catch( exception &e )
    {
        T_LOG( e.what() );
        return (-1);
    }
    if( Debug ) T_END;

    return 0;  // success
}

// called by every child
regionCondShm* mapSharedMem( int fd )
{
    if( Debug ) T_START;
    // memory map the shared memory
    regionCondShm *rptr = nullptr;
    long pg_size = sysconf( _SC_PAGE_SIZE );
    assert( pg_size >= (long ) (sizeof(struct regionCondShm)) );// make sure a page is larger than regionCondShm

    rptr = (regionCondShm*) (mmap( NULL, pg_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ));
    if( rptr == MAP_FAILED )
    {
        throwARunTimeError( "mmap()", true, &(rptr->numErrors), __LINE__ );
    }
    if( Debug) T_END;

    return rptr;
}

// called by main thread to initialize shared memory
void initSharedMem( regionCondShm* const rptr )
{
    T_START;
    int ret = 0;
    strncpy( rptr->testName, "ConditionVariablesTest", BUFSIZE );
    rptr->numErrors = 0;
    rptr->numProcsInitialized = 0;

    // Initialize mutex and condition variable.
    pthread_mutexattr_t attr;
    if( (ret = pthread_mutexattr_init( &attr )) )
    {

        throwARunTimeError( "pthread_mutexattr_init()", true, &(rptr->numErrors), __LINE__, ret );
    }

    //  note: PTHREAD_PROCESS_SHARED not supported in Cygwin because of lack of
    //  support in windows Pthreads-w32
    if( (ret = pthread_mutexattr_setpshared( &attr, PTHREAD_PROCESS_SHARED )) )
    {
        if( ret == EINVAL )
        {
            throwARunTimeError( "pthread_mutexattr_setpshared()", false, &(rptr->numErrors), __LINE__, ret );
        } else
        {
            throwARunTimeError( "pthread_mutexattr_setpshared()", true, &(rptr->numErrors), __LINE__, ret );
        }
    }
    if( ( ret = pthread_mutex_init( &rptr->count_mutex, &attr ) ) )
    {
        throwARunTimeError( "pthread_mutex_init()", true, &(rptr->numErrors), __LINE__, ret );
    }
    if( ( ret = pthread_mutexattr_destroy( &attr ) ) )
    {
        throwARunTimeError( "pthread_mutexattr_destroy()", true, &(rptr->numErrors), __LINE__, ret );
    }

    // Initialize condition variable
    pthread_condattr_t condAttr;
    if( ( ret = pthread_condattr_init( &condAttr ) ) )
    {
        throwARunTimeError( "pthread_condattr_init()", true, &(rptr->numErrors), __LINE__, ret );
    }
    //  note: PTHREAD_PROCESS_SHARED not supported in Cygwin because of lack of
    //  support in windows Pthreads-w32
    if( (ret = pthread_condattr_setpshared( &condAttr, PTHREAD_PROCESS_SHARED )) )
    {
        if( ret == EINVAL )
        {
            throwARunTimeError( "pthread_condattr_setpshared()", false, &(rptr->numErrors), __LINE__, ret );
        } else
        {
            throwARunTimeError( "pthread_condattr_setpshared()", true, &(rptr->numErrors), __LINE__, ret );
        }
    }
    if( ( ret = pthread_condattr_setclock( &condAttr, CLOCK_MONOTONIC ) ) )
    {
        throwARunTimeError( "pthread_condattr_setclock()", true, &(rptr->numErrors), __LINE__, ret );
    }
    if( ( ret = pthread_cond_init( &rptr->all_processes_created, &condAttr ) ) )
    {
        throwARunTimeError( "pthread_cond_init()", true, &(rptr->numErrors), __LINE__ , ret );
    }
    if( ( ret = pthread_condattr_destroy( &condAttr ) ) )
    {
        throwARunTimeError( "pthread_condattr_destroy()", true, &(rptr->numErrors), __LINE__, ret );
    }

    T_END;
}

void coordinateTests( regionCondShm* const rptr )
{
    T_START;
    // no tests defined..
    T_END;
}

void createProcesses( pid_t* pidTble )
{
    T_START;
    // create processes
    for( unsigned int procNum = 0; procNum < NUM_PROC; procNum++ )
    {

        pid_t pid = fork();
        if( pid == -1 )
        {
            throwARunTimeError( "fork()", false, nullptr, __LINE__ );
            assert( false );
        }
        else if( pid == 0 )
        {
            int ret = doChildActions( procNum );
            exit( ret );
        } else
        {
            // parent
            pidTble[procNum] = pid;
        }
    }
    // sleep to give child processes time to initialize.
    sleep( 5 );
    T_END;
}

void waitForProcess( pid_t* pidTble )
{
    T_START;
    // wait for processes
    for( unsigned int i = 0; i < NUM_PROC; i++ )
    {
        int result = 0;
        pid_t pid;
        if( (pid = waitpid( pidTble[i], &result, 0 )) == -1 )
        {
            throwARunTimeError( "waitpid()", false, nullptr, __LINE__ );
            assert( false );
        }
        assert( pid == pidTble[i] );
    }
    T_END;
}

// wait for all processes to complete initialization and startup.
void waitForAllProcessesToInit( regionCondShm* const rptr )
{
    if( Debug ) T_START;
    int ret = 0;
    if( ( ret = pthread_mutex_lock( &rptr->count_mutex ) ) )
    {
        throwARunTimeError( "pthread_mutex_lock()", false, &(rptr->numErrors), __LINE__, ret );
        rptr->numErrors++;
    }

    rptr->numProcsInitialized++;
    if( Debug )
    {
        printSharedMem( rptr );
    }

    //  Wait until the last process  has started. Then release other processes.
    if( rptr->numProcsInitialized >= NUM_PROC )
    {
        // release all waiting threads.
        T_LOG( "broadcast!!" );

        if( ( ret = pthread_cond_broadcast( &rptr->all_processes_created ) ) )
        {
            throwARunTimeError( "pthread_cond_broadcast()", true, &(rptr->numErrors), __LINE__, ret );
        }
    } else
    {
        // wait till last thread is created.
        if( Debug ) T_LOG( "wait" );

        // settimeout 2seconds.
        struct timespec ts;
        clock_gettime( CLOCK_MONOTONIC, &ts );
        ts.tv_sec += 2;
        //       note: pthread_cond_timedwait() always times out since Windows does not support
        //       PTHREAD_SHARED
        if( (ret = pthread_cond_timedwait( &rptr->all_processes_created, &rptr->count_mutex, &ts )) )
        {
            if( ret == ETIMEDOUT )
            {
                if( !(rptr->numTimeoutErrors) ) //  only throw for one error of this type; reduces logging
                {
                    throwARunTimeError( "pthread_cond_timedwait()", false, &(rptr->numErrors), __LINE__, ret );
                }
                rptr->numTimeoutErrors++;
            }
            else
            {
                throwARunTimeError( "pthread_cond_timedwait()", true, &(rptr->numErrors), __LINE__, ret );
            }
        }
    }
    if( ( ret = pthread_mutex_unlock( &rptr->count_mutex ) ) )
    {
        throwARunTimeError( "pthread_mutex_unlock()", false, &(rptr->numErrors), __LINE__, ret );
    }
    if( Debug ) T_END;
}


void cleanupSharedMem( regionCondShm** rptrptr )
{
    T_START;
    int ret = 0;

    regionCondShm* rptr = *rptrptr;
    if( rptr )
    {
        if ( ( ret = pthread_mutex_destroy( &(rptr->count_mutex ) ) ) )
        {
            throwARunTimeError( "pthread_mutex_destroy()", false, &(rptr->numErrors), __LINE__, ret );
        }
        if( ( ret = pthread_cond_destroy( &(rptr->all_processes_created) ) ) )
        {
            throwARunTimeError( "pthread_cond_destroy()", false, &(rptr->numErrors), __LINE__, ret );
        }
        if( munmap( rptr, sizeof(struct regionCondShm) ) )
        {
            throwARunTimeError( "munmap()", false, &(rptr->numErrors), __LINE__ );
        }
    }
    rptr = nullptr;
    T_END;
    return;
}

void flushToDisk( regionCondShm* const rptr )
{
    T_START;
    if( msync( rptr, sizeof(struct regionCondShm), MS_SYNC ) )
    {
        throwARunTimeError( "msync()", true, &(rptr->numErrors), __LINE__ );
    }

    T_END;
}

void printSharedMem( regionCondShm* const rptr )
{
    // print contents of shared memory
    stringstream ss;
    ss << "shm at " << rptr << "\n";
    ss << "testName=" << rptr->testName << "\n";
    ss << "numProcsInitialized=" << rptr->numProcsInitialized << "\n";
    ss << "numErrors=" << rptr->numErrors << "\n";
    ss << "numTimeoutErrors=" << rptr->numTimeoutErrors;

    T_LOG( ss.str().c_str() );
}


