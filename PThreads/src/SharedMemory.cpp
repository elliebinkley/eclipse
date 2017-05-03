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

const unsigned int NUM_PROC = 90;   // 256 is the max number of processes allowed; see ulimit -a
const unsigned int ITERATIONS = 1000;
const unsigned int NUM_TESTS = 5;

const char* MEMORY_NAME = "/memory_name";
const char* TOTAL_SUM_SEM = "/total_sum_named_semaphore";   // named semaphore

bool Debug = false;
int fd;

/* Shared memory structures  */
struct testInstance
{
    sem_t startTest;
    sem_t finishTest;
    char name[100];
    volatile std::atomic<unsigned int> numProcsFinishedTest;
    volatile std::atomic<uint64_t> elapsedTime;
    volatile std::atomic<unsigned long> sumResult;
};

struct region
{ /* Defines "structure" of shared memory */
    volatile std::atomic<unsigned long> sum[NUM_PROC];  // test 0
    volatile unsigned long totalSum;                    // test 1
    volatile std::atomic<unsigned long> totalSumAtomic; // test 2
    volatile unsigned long totalSum_Unamed_Sem;         // test 3; protected by unNamedSem
    volatile unsigned long totalSum_Named_Sem;   // test 4; protected by "total_sum_named_semaphore"
    volatile std::atomic<unsigned int> numProcsInitialized;
    volatile std::atomic<unsigned int> numErrors;
    pthread_mutex_t count_mutex;                 // condition variable mutex
    pthread_cond_t all_processes_created;        // condition variable.
    sem_t unNamedSem;                            // unnamed Semaphore
    struct testInstance test[NUM_TESTS];         //
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
static void printSharedMem( region* const rptr );
static sem_t* initSemaphores( region* const rptr );
static void closeSemaphores( region* const rptr, sem_t* namedSemPtr );
static void signalIfTestsDone( region* const rptr, unsigned int i );
static void signalStartTest( region* const rptr, unsigned int i );
static void waitForStartTestSignal( region* const rptr, unsigned int i );
static void waitForTestToFinishSignal( region* const rptr, timespec ts_start, unsigned int i );
static void coordinateTests( region* const rptr );

static void throwARunTimeError( const char* functionName, bool doThrow, region* const rptr,
                                int line );

using namespace std;

void sharedMemory()
{
    T_START;
    pid_t pidTble[NUM_PROC];
    region* rptr = nullptr;
    sem_t* namedSemPtr = nullptr;
    try
    {
        fd = openShm( true );
        rptr = mapSharedMem( fd );
        initSharedMem( rptr );
        namedSemPtr = initSemaphores( rptr );

        createProcesses( pidTble );
        coordinateTests( rptr );
        waitForProcess( pidTble );

        assert( checkShm( rptr ) ); // check sums

        flushToDisk( rptr );
        printSharedMem( rptr );
        closeSemaphores( rptr, namedSemPtr );
        cleanupSharedMem( &rptr );

        // get rid of shared memory
        if( shm_unlink(MEMORY_NAME) )
        {
            throwARunTimeError("shm_unlink()", true, nullptr, __LINE__ );
        }
        if( sem_unlink(TOTAL_SUM_SEM ) )
        {
            throwARunTimeError("sem_unlink()", true, nullptr, __LINE__ );
        }
        T_LOG( " SUCCESS!! ");
    }
    catch ( std::exception &ex)
    {
        T_LOG("caught exception");
        closeSemaphores( rptr, namedSemPtr );
        cleanupSharedMem( &rptr );
        shm_unlink( MEMORY_NAME );
        sem_unlink( TOTAL_SUM_SEM );
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

int doChildActions( unsigned int procNum )
{
    //   T_START;
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
        region* const rptr = mapSharedMem( fd );

        // get Named semaphore
        sem_t* namedSemPtr;
        if( (namedSemPtr = sem_open( TOTAL_SUM_SEM, O_RDWR )) == SEM_FAILED )
        {
            throwARunTimeError( "sem_open()", true, nullptr, __LINE__ );
        }

        waitForAllProcessesToInit( rptr );

        // test 0
        unsigned int testId = 0;
        waitForStartTestSignal( rptr, testId );

        stringstream ss;
        ss << "running test[" << testId << "]=" << rptr->test[testId].name;
        T_LOG( ss.str().c_str() );
        for( unsigned int k = 0; k < ITERATIONS; k++ )
        {
            rptr->sum[procNum]++;                 // safe, since process specific.
        }
        signalIfTestsDone( rptr, testId );

        // test 1
        testId++;
        assert( testId <= NUM_TESTS );
        waitForStartTestSignal( rptr, testId );

        ss.str("");
        ss << "running test[" << testId << "]=" << rptr->test[testId].name << "]";
        T_LOG( ss.str().c_str() );
        for( unsigned int k = 0; k < ITERATIONS; k++ )
        {
            rptr->totalSum++;                // not process/thread-safe..
        }
        signalIfTestsDone( rptr, testId );

        // test 2
        testId++;
        assert( testId <= NUM_TESTS );
        waitForStartTestSignal( rptr, testId );
        ss.str("");
        ss << "running test[" << testId << "]=" << rptr->test[testId].name << "]";
        T_LOG( ss.str().c_str() );
        for( unsigned int k = 0; k < ITERATIONS; k++ )
        {
            rptr->totalSumAtomic++;         //  safe since protected with std::atomic<>
        }
        signalIfTestsDone( rptr, testId );

        // test 3
        testId++;
        assert( testId <= NUM_TESTS );
        waitForStartTestSignal( rptr, testId );
        ss.str("");
        ss << "running test[" << testId << "]=" << rptr->test[testId].name << "]";
        T_LOG( ss.str().c_str() );
        for( unsigned int k = 0; k < ITERATIONS; k++ )
        {
            sem_wait( namedSemPtr );
            rptr->totalSum_Named_Sem++;       // protected with named sem
            sem_post( namedSemPtr );
        }
        signalIfTestsDone( rptr, testId );

        // test 4
        testId++;
        assert( testId <= NUM_TESTS );
        waitForStartTestSignal( rptr, testId );
        ss.str("");
        ss << "running test[" << testId << "]=" << rptr->test[testId].name << "]";
        T_LOG( ss.str().c_str() );
        for( unsigned int k = 0; k < ITERATIONS; k++ )
        {
            sem_wait( &(rptr->unNamedSem) );
            rptr->totalSum_Unamed_Sem++;      // protected with unnamed sem
            sem_post( &(rptr->unNamedSem) );
        }
        signalIfTestsDone( rptr, testId );

        if( Debug )
        {
            printSharedMem( rptr );
        }

        closeSemaphores( nullptr, namedSemPtr );

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

    for( unsigned int i = 0; i < NUM_TESTS; i++ )
    {
        rptr->test[i].numProcsFinishedTest = 0;
        rptr->test[i].sumResult = 0;

        if( sem_init( &(rptr->test[i].finishTest), 1, 0 ) == -1 )
        {
            throwARunTimeError( "sem_open()", true, nullptr, __LINE__ );
        }
        if( sem_init( &(rptr->test[i].startTest), 1, 0 ) == -1 )
        {
            throwARunTimeError( "sem_open()", true, nullptr, __LINE__ );
        }
        switch( i )
        {
        case 0:
        {
            for( unsigned int k = 0; k < NUM_PROC; k++ )
            {
                rptr->sum[k] = 0;
            }
            strcpy( rptr->test[i].name, "sumForEachProcess" );

            break;
        }
        case 1:
        {
            strcpy( rptr->test[i].name, "totalSum" );
            rptr->totalSum = 0;
            break;
        }
        case 2:
        {
            rptr->totalSumAtomic = 0;
            strcpy( rptr->test[i].name, "totalSumAtomic" );
            break;
        }
        case 3:
        {
            rptr->totalSum_Named_Sem = 0;
            strcpy( rptr->test[i].name, "totalSum_Named_Sem" );
            break;
        }
        case 4:
        {
            rptr->totalSum_Unamed_Sem = 0;
            strcpy( rptr->test[i].name, "totalSum_UNnamed_Sem" );
            break;
        }
        default:
        {
            assert( false );
            break;
        }
        }
    }
    rptr->numErrors = 0;
    rptr->numProcsInitialized = 0;

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

sem_t* initSemaphores( region* const rptr )
{
    // init named semaphore
    sem_t* namedSemPtr;
    int oflag = (O_CREAT | O_EXCL | O_RDWR);
    if( (namedSemPtr = sem_open( TOTAL_SUM_SEM, oflag, 0644, 1 )) == SEM_FAILED )
    {
        throwARunTimeError( "sem_open()", true, nullptr, __LINE__ );
    }

    // init unnamed sempahore
    if( sem_init( &(rptr->unNamedSem), 1, 1 ) == -1 )
    {
        throwARunTimeError( "sem_open()", true, nullptr, __LINE__ );
    }

    T_END;
    return namedSemPtr;
}

void closeSemaphores( region* const rptr, sem_t* namedSemPtr )
{
    // destroy unnamed semaphore;
    if( rptr )
    {
        if( sem_destroy( &(rptr->unNamedSem) ) )
        {
            throwARunTimeError( "sem_destroy()", false, nullptr, __LINE__ );
        }
    }

    // close named semaphore
    if( namedSemPtr )
    {
        if( sem_close( namedSemPtr ) )
        {
            throwARunTimeError( "sem_close()", false, nullptr, __LINE__ );
        }
    }
}

// processes call this to wait for the start of a new test
void waitForStartTestSignal( region* const rptr, unsigned int i )
{
    T_START;
    if( sem_wait( &(rptr->test[i].startTest) ) )
    {
        string s("sem_wait() on startTest failed; perror=");
        s.append( strerror(errno) );
        T_LOG( s.c_str() );
        throw( s.c_str() );
    }
    T_END;
}

// processes call this when their test is done.
void signalIfTestsDone( region* const rptr, unsigned int i )
{
    T_START;
    if( ++(rptr->test[i].numProcsFinishedTest) >= NUM_PROC )
    {
        // signal main thread that all children are done.
        if( sem_post( &(rptr->test[i].finishTest) ))
        {
            string s("sem_post() on finishTest failed; perror=");
            s.append( strerror(errno) );
            T_LOG( s.c_str() );
            throw( s.c_str() );
        }
    }
    T_END;
}

// main thread calls this to wait for the finish of the test
void waitForTestToFinishSignal( region* const rptr, timespec ts_start, unsigned int testId )
{
    T_START;
    int ret;
    if( (ret = sem_wait( &(rptr->test[testId].finishTest) )))
    {
        string s("sem_wait() on finishTest failed; perror=");
        s.append( strerror(errno) );
        T_LOG( s.c_str() );
        throw( s.c_str() );
    }
    // do the accounting...
    struct timespec ts_finish;
    clock_gettime( CLOCK_MONOTONIC, &ts_finish );
    uint64_t delta_us = (ts_finish.tv_sec - ts_start.tv_sec) * 1000000 + (ts_finish.tv_nsec - ts_start.tv_nsec) / 1000;
    stringstream ss;
    rptr->test[testId].elapsedTime = delta_us*1000;
    ss << rptr->test[testId].name << ":elapsed time=" << ( delta_us*1000 ) << " ms";
    T_LOG( ss.str().c_str() );
    T_END;
}

// main thread calls this to signal the start of a test.
void signalStartTest( region* const rptr, unsigned int i )
{
    T_START;
    struct timespec ts_start;
    clock_gettime( CLOCK_MONOTONIC, &ts_start );

    for( unsigned int k =0; k < NUM_PROC; k++ )
    {
        int ret;
        if( ( ret = sem_post( &(rptr->test[i].startTest)) ))
        {
            string s("sem_post() failed on startTest; perror=");
            s.append( strerror(errno) );
            T_LOG( s.c_str() );
            throw( s.c_str() );
        }
    }
    waitForTestToFinishSignal( rptr, ts_start, i );
    T_END;
}

void coordinateTests( region* const rptr )
{
    for( unsigned int testId; testId < NUM_TESTS; testId++ )
    {
        sleep( 5 );
        stringstream ss;
        ss << "starting test[" << testId << "] = " << rptr->test[testId].name;
        T_LOG( ss.str().c_str() );
        signalStartTest( rptr, testId );
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
    if( sum == 0 )
    {
        T_LOG( "SUM is zero; test failed" );
        return false;
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
    for( unsigned int procNum = 0; procNum < NUM_PROC; procNum++ )
    {
        pid_t pid = fork();
        if( pid == -1 )
        {
            throwARunTimeError( "fork()", false, nullptr, __LINE__ );
            assert( false );
        } else if( pid == 0 )
        {
            int ret = doChildActions( procNum );
            exit( ret );
        } else
        {
            // parent
            pidTble[procNum] = pid;
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
        //     assert( result == 0 );
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

    //  set the sums for each test for uniform printing
    for( unsigned int k = 0; k < NUM_PROC; k++ )
    {
        rptr->test[0].sumResult += rptr->sum[k];
    }
    rptr->test[1].sumResult += rptr->totalSum;
    rptr->test[2].sumResult += rptr->totalSumAtomic;
    rptr->test[3].sumResult += rptr->totalSum_Named_Sem;
    rptr->test[4].sumResult += rptr->totalSum_Unamed_Sem;

    // print
    stringstream ss;
    ss << "shm at " << rptr << endl;
    ss << " numProcsInitialized=" << rptr->numProcsInitialized << endl;
    for( unsigned int procId = 0; procId < NUM_PROC; procId++ )
    {
        ss << " sum[" << procId << "]=" << rptr->sum[procId] << endl;
    }

    uint64_t base = rptr->test[0].elapsedTime;
    for( unsigned int testId = 0; testId < NUM_TESTS; testId++ )
    {
        ss << "test[=" << testId << "]= " << rptr->test[testId].name << endl;
        ss << "test[=" << testId << "] Sum = " << rptr->test[testId].sumResult;
        unsigned long ratio = rptr->test[testId].elapsedTime / base;
        ss << "      elapsedTime = " << rptr->test[testId].elapsedTime << " ms ;" << " ratio = "
                << ratio << endl;
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
