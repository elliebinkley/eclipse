/*
 *  SharedMemory.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: Burley
 *      1.Create shared memory ( shm_open()  ) with a named location.  Produced a file under /def/shm ..
 *      2.Map shared memory mmap().
 *        Note that mlock() does not work under Cygwin.
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
 *      5. Each forked access accesses shared memory via mmap() with the inherited file descriptor. .
 *      6. Increment several variables ; e.g. using atomics and not using atomics.
 *      7. Once all forked processes have finished, the manager process will check if the sums match.
 *
 *      References:
 *      1. shm_open,  shm_unlink  -  create/open  or  unlink POSIX shared memory
 *      2. mmap, munmap, mmsync() - map or unmap files or sync devices into memory
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

#include <cstdio>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <fcntl.h>           /* For O_* constants */

#include <string>
#include <sstream>
#include <iostream>
#include <atomic>

#include "SharedLibrary/inc/MyLogger.hpp"

const unsigned int NUM_PROC = 30;   // 256 is the max number of processes allowed; see ulimit -a
const unsigned int ITERATIONS = 10000000;

const char* MEMORY_NAME = "/memory_name";
bool Debug = false;
int fd;

/* Map shared memory object */
struct region
{ /* Defines "structure" of shared memory */
    volatile std::atomic<unsigned long> sum[NUM_PROC];
    volatile std::atomic<unsigned long> totalSumAtomic;
    volatile std::atomic<unsigned int> numProcsInitialized;
    volatile unsigned long totalSum;
    volatile std::atomic<unsigned int> numErrors;
    pthread_mutex_t count_mutex;                 // condition variable mutex
    pthread_cond_t all_processes_created;        // condition variable.
};

// functions in this file
static int doChildActions( unsigned int i );
static int openShm( bool create );
static bool checkShm( region* const rptr );
static region* mapSharedMem( int fd );
static void createProcesses( pid_t* pidTble );
static void waitForAllProcessesToInit( region* const rptr );
static void waitForProcess( pid_t* pidTble );
static void initSharedMem( region* const rptr );
static void flushToDisk( region* const rptr );
static void cleanupSharedMem( region** const rptr );
void printSharedMem( region* const rptr );
static void throwARunTimeError( const char* functionName, bool doThrow, region* const rptr,
                                int line );

using namespace std;

void sharedMemory()
{
    T_START;
    pid_t pidTble[NUM_PROC];
    region* rptr = nullptr;
    try
    {
        fd = openShm( true );
        rptr = mapSharedMem( fd );
        initSharedMem( rptr );
        createProcesses( pidTble );
        waitForProcess( pidTble );

        assert( checkShm( rptr ) ); // check sums

        flushToDisk( rptr );
        printSharedMem( rptr );
        cleanupSharedMem( &rptr );

        // get rid of shared memory
        if( shm_unlink(MEMORY_NAME) )
        {
            throwARunTimeError("shm_unlink()", true, nullptr, __LINE__ );
        }
        T_LOG( " SUCCESS!! ");
    }
    catch ( std::exception &ex)
    {
        T_LOG("caught exception");
        cleanupSharedMem( &rptr );
        shm_unlink(MEMORY_NAME);
        T_LOG( ex.what() );
    }

    T_END;
}

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
    if (ftruncate(fd, sizeof(struct region)) == -1)
    {
        throwARunTimeError("ftruncate()", true, nullptr, __LINE__ );
    }

    T_END;
    return fd;
}

int doChildActions( unsigned int i )
{
    //   T_START;
    try
    {
        // print out pid and i number
        if( Debug )
        {
            stringstream ss;
            ss << "i=" << i << " pid=" << getpid() << " &fd=" << &fd;
            T_LOG( ss.str().c_str() );
        }

        // memory map the shared memory
        region *rptr = mapSharedMem( fd );

        waitForAllProcessesToInit( rptr );

        for( unsigned int k = 0; k < ITERATIONS; k++ )
        {
            rptr->totalSum++;       // not process safe..
            rptr->sum[i]++;         // safe, since process specific.
            rptr->totalSumAtomic++;         //  safe since protected with std::atomic<>
        }
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
    //   T_END;
    return 0;  // success
}

region* mapSharedMem( int fd )
{
    // memory map the shared memory
    region *rptr = nullptr;
    long pg_size = sysconf( _SC_PAGE_SIZE );
    assert( pg_size >= (long ) (sizeof(struct region)) ); // make sure a page is larger than what we need

    rptr = (region*) (mmap( NULL, pg_size,
    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ));
    if( rptr == MAP_FAILED )
    {
        throwARunTimeError( "mmap()", true, rptr, __LINE__ );
    }

    // mlock() does not work for memory mapped regions.  Works for the heap though.. Weird.
    region* ptr = (region*) malloc( sizeof(struct region) );
    // lock into memory; disable paging...
    if( mlock( ptr, sizeof(struct region) ) )
    {
        throwARunTimeError( "mlock()", true, rptr, __LINE__ );
    }
    if( munlock( ptr, sizeof(struct region) ) )
    {
        throwARunTimeError( "mlock()", true, rptr, __LINE__ );
    }
    free( ptr );

    return rptr;
}

void initSharedMem( region* const rptr )
{
    int ret = 0;
    rptr->numProcsInitialized = 0;
    rptr->totalSum = 0;
    for( unsigned int i = 0; i < NUM_PROC; i++ )
    {
        rptr->sum[i] = 0;
    }

    // Initialize mutex
    pthread_mutexattr_t attr;
    if( (ret = pthread_mutexattr_init( &attr )) )
    {
        throwARunTimeError( "pthread_mutexattr_init()", true, rptr, __LINE__ );
    }

    //  note: PTHREAD_PROCESS_SHARED not supported in Cygwin because of lack of
    //  support in windows Pthreads-w32
    if( (ret = pthread_mutexattr_setpshared( &attr, PTHREAD_PROCESS_SHARED )) )
    {
        if( ret == EINVAL )
        {
            string s = "pthread_mutexattr_setpshared(); ret=";
            s.append( strerror( ret ) );
            s.append( " ... continuing ..." );
            T_LOG( s.c_str() );
        } else
        {
            throwARunTimeError( "pthread_mutexattr_setpshared()", false, rptr, __LINE__ );
        }
    }
    if( pthread_mutex_init( &rptr->count_mutex, &attr ) )
    {
        throwARunTimeError( "pthread_mutex_init()", true, rptr, __LINE__ );
    }
    if( pthread_mutexattr_destroy( &attr ) )
    {
        throwARunTimeError( "pthread_mutexattr_destroy()", true, rptr, __LINE__ );
    }

    // Initialize condition variable
    pthread_condattr_t condAttr;
    if( pthread_condattr_init( &condAttr ) )
    {
        throwARunTimeError( "pthread_condattr_init()", true, rptr, __LINE__ );
    }
    //  note: PTHREAD_PROCESS_SHARED not supported in Cygwin because of lack of
    //  support in windows Pthreads-w32
    if( (ret = pthread_condattr_setpshared( &condAttr, PTHREAD_PROCESS_SHARED )) )
    {
        if( ret == EINVAL )
        {
            string s = "pthread_condattr_setpshared(); ret=";
            s.append( strerror( ret ) );
            s.append( " ... continuing ..." );
            T_LOG( s.c_str() );
        } else
        {
            throwARunTimeError( "pthread_condattr_setpshared()", false, rptr, __LINE__ );
        }
    }
    if( pthread_condattr_setclock( &condAttr, CLOCK_MONOTONIC ) )
    {
        throwARunTimeError( "pthread_condattr_setclock()", true, rptr, __LINE__ );
    }
    if( pthread_cond_init( &rptr->all_processes_created, &condAttr ) )
    {
        throwARunTimeError( "pthread_cond_init()", true, rptr, __LINE__ );
    }
    if( pthread_condattr_destroy( &condAttr ) )
    {
        throwARunTimeError( "pthread_condattr_destroy()", true, rptr, __LINE__ );
    }
}

// check totals; the only one we expect to match is totalSumAtomic.
bool checkShm( region* const rptr )
{
    unsigned int sum = 0;
    for( unsigned int i = 0; i < NUM_PROC; i++ )
    {
        sum += rptr->sum[i];
    }

    stringstream ss;
    if( sum != rptr->totalSum )
    {
        if( Debug )
        {
            printSharedMem( rptr );
        }
        ss << "sums don't match: rptr->totalSum=" << rptr->totalSum << " sum=" << sum
                << " difference=" << sum - rptr->totalSum;
    } else
    {
        ss << "sums match: rptr->totalSum=" << rptr->totalSum;
    }
    T_LOG( ss.str().c_str() );

    stringstream ss1;
    bool match;
    if( sum != rptr->totalSumAtomic )
    {
        printSharedMem( rptr );
        match = false;
        ss1 << "sums don't match: rptr->totalSumAtomic=" << rptr->totalSumAtomic << " sum=" << sum;
    } else
    {
        match = true;
        ss1 << "sums match: rptr->totalSumAtomic=" << rptr->totalSumAtomic;
    }
    T_LOG( ss1.str().c_str() );
    return match;
}

void createProcesses( pid_t* pidTble )
{
    // create processes
    for( unsigned int i = 0; i < NUM_PROC; i++ )
    {
        pid_t pid = fork();
        if( pid == -1 )
        {
            throwARunTimeError( "fork()", false, nullptr, __LINE__ );
            assert( false );
        } else if( pid == 0 )
        {
            int ret = doChildActions( i );
            exit( ret );
        } else
        {
            // parent
            pidTble[i] = pid;
        }
    }
}

void waitForProcess( pid_t* pidTble )
{
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
        assert( result == 0 );
    }
}

void waitForAllProcessesToInit( region* const rptr )
{
    if( pthread_mutex_lock( &rptr->count_mutex ) )
    {
        throwARunTimeError( "pthread_mutex_lock()", false, rptr, __LINE__ );
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
        if( Debug ) T_LOG( "cond" );

        if( pthread_cond_broadcast( &rptr->all_processes_created ) )
        {
            throwARunTimeError( "pthread_cond_broadcast()", true, rptr, __LINE__ );
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
        int ret;
        if( (ret = pthread_cond_timedwait( &rptr->all_processes_created, &rptr->count_mutex, &ts )) )
        {
            if( ret == ETIMEDOUT )
            {
                string s = "pthread_cond_timedwait(); ret=";
                s.append( strerror( ret ) );
                s.append( " ... continuing ..." );
                T_LOG( s.c_str() );
            } else
            {
                throwARunTimeError( "pthread_cond_timedwait()", true, rptr, __LINE__ );
            }
        }
    }
    if( pthread_mutex_unlock( &rptr->count_mutex ) )
    {
        throwARunTimeError( "pthread_mutex_unlock()", false, rptr, __LINE__ );
    }
}

void cleanupSharedMem( region** rptr )
{
    if( *rptr )
    {
        pthread_mutex_destroy( &((*rptr)->count_mutex) );
        pthread_cond_destroy( &((*rptr)->all_processes_created) );
        if( munmap( *rptr, sizeof(struct region) ) )
        {
            throwARunTimeError( "munmap()", false, *rptr, __LINE__ );
        }
    }
    *rptr = nullptr;
}

void flushToDisk( region* const rptr )
{
    T_START;
    if( msync( rptr, sizeof(struct region), MS_SYNC ) )
    {
        throwARunTimeError( "msync()", true, rptr, __LINE__ );
    }

    T_END;
}

void printSharedMem( region* const rptr )
{
    stringstream ss;
    ss << "shm at " << rptr << endl;
    ss << " numProcsInitialized=" << rptr->numProcsInitialized << endl;
    ss << " totalSum=" << rptr->totalSum << endl;
    ss << " totalSumAtomic=" << rptr->totalSumAtomic << endl;
    for( unsigned int i = 0; i < NUM_PROC; i++ )
    {
        ss << " sum[" << i << "]=" << rptr->sum[i] << endl;
    }
    T_LOG( ss.str().c_str() );
}

void throwARunTimeError( const char* functionName, bool doThrow, region* const rptr, int line )
{
    if( rptr ) rptr->numErrors++;
    stringstream ss;
    ss << "Line#=" << line << " " << functionName << " failed; perror=" << strerror( errno );
    if( doThrow )
    {
        throw(std::runtime_error( ss.str().c_str() ));
    } else
    {
        ss << "..continuing..";
    }
    T_LOG( ss.str().c_str() );
    return;
}
