//
// IpcXsi.cpp
//
//  Created on: Apr 6, 2017
//  Author: Burley
// Description : IPC exercises.
//             : 1. create a bunch of threads and synchronize them using unnamed semaphores.
//                  syncPosixThreadsWithUnnamedPosixSemaphore();
//               2. create a bunch of threads and synchronize them using named semaphores.
//                  syncProcessesWithNamedPosixSemaphore()
//
//               3. Note that in cygwin the /usr/sbin/cygserver.exe windows service must be started to allow IPC operations.
//                  The cygserver supports persistence of IPC data structures; e.g. names semaphores, message queues, shared memory.
//                  See https://cygwin.com/cygwin-ug-net/using-cygserver.html
//                  cygserver supports XSI IPC Message Queues,  XSI IPC Semaphores, and XSI IPC Shared
//                  Memory ( sys/ipc.h, sys/mman.h, sys/sem.h ). These are originally Unix System V and are not posix compliant.
//                  They are kernel space objects, so if the process is killed or terminated unexpected, they can be preserved.
//                  But they are slow since they require a round trip to call into the kernel.
//
//                  <semaphore.h> defines POSIX semaphores, which are designed in such a way that they can be implemented entirely
//                  in userspace.
//                  POSIX semaphores also offer the choice of whether you want a process-local semaphore
//                  (for use in a multi-threaded
//                  environment, or even, in some conditions, a signal handler in a single-threaded program) or a
//                  process-shared one, and in the latter case, you also have the choice whether to let the system handle
//                  allocating it in a shared namespace by name,
//                  or to obtain shared memory yourself and initialize it in shared memory.
//
//                  Under cygwin, a created named semaphore is located under /dev/shm/sem directory.
//
//                  ( See http://stackoverflow.com/questions/11058045/differences-between-semaphore-h-and-sys-sem-h )
//                  ( See http://man7.org/linux/man-pages/man7/sem_overview.7.html for an overview of semaphores.
//                  See sem_wait(), sem_trywait(), sem_post(), and sem_destroy() sem_close, sem_unlink()
//

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>      // O_CREAT

#include <iostream>
#include <sstream>

#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;

#define SEM_NAME "/mysem"  // named semaphore

static const unsigned int NUM_PROC = 3;
static const unsigned int NUM_THREAD = 3;

static void* threadAction( void* );
static void syncProcessesWithNamedPosixSemaphore();
static void syncPosixThreadsWithUnnamedPosixSemaphore();
static int runChildProcesses();

void ipcXsi()
{
    T_START;

    syncPosixThreadsWithUnnamedPosixSemaphore();
    syncProcessesWithNamedPosixSemaphore();

    T_END;
    pthread_exit(NULL);
}

void syncProcessesWithNamedPosixSemaphore()
{
    T_START;

    pid_t pid[NUM_PROC];
    sem_t *sem = NULL;

    try
    {
        // part 1. create semaphore
        int initAttempts = 0;
        bool initGood = false;
        while( !initGood && initAttempts++ < 2 )
        {
            // open and create the names semaphore with a value of 1. If already present, this does nothing.
            sem = sem_open(SEM_NAME, O_CREAT, 0644, 1 );
            if( sem == SEM_FAILED )
            {
                string s="sem_open() failed;errno=";
                s.append(strerror(errno));
                T_LOG( s.c_str());
                throw( std::runtime_error( s.c_str()));
            }

            // check value; if the persistent semapahore was already created prior to this program running
            // and had a value other than 1, destroy it and start over...
            int sval = 0;
            if( sem_getvalue(sem, &sval) )
            {
                string s = "sem_getvalue() failed;errno=";
                s.append(strerror(errno));
                T_LOG(s.c_str());
                throw std::runtime_error(s.c_str());
            }
            else
            {
                if( sval != 1 )
                {
                    stringstream ss;
                    ss << "destroying and re-creating semaphore; semValue=" << sval;
                    T_LOG(ss.str());
                    sem_unlink( SEM_NAME );  // destroys persistent semaphore
                    continue;
                }
                else
                {
                    stringstream ss;
                    ss << "Semaphore=" << sem;
                    T_LOG(ss.str());
                    initGood = true;
                }
            }
        }
        // part 2. fork processes
        for( unsigned int i = 0; i < NUM_PROC; i++ )
        {
            pid[i] = fork();
            if (pid[i] == 0)
            {
                // I am the child
                //execvp ("./child", NULL);
                exit( runChildProcesses() );
            }
            else if( pid[i] == -1 ) // fork error
            {
                string s = "fork() error=";
                s.append(strerror(errno));
                T_LOG(s.c_str());
                throw( runtime_error( s.c_str()));
            }
        }

        // I am the parent
        // part 3. wiat for child processes.
        T_LOG("Parent: Waiting for Child to complete.");

        int status, ret;
        for( unsigned int i = 0; i < NUM_PROC; i++ )
        {
            if ( (ret = waitpid( pid[i], &status, 0)) == -1 )
            {
                string s = "waitpid() error=";
                s.append(strerror(errno));
                T_LOG(s.c_str());
            }

            if( ret == pid[i] )
            {
                stringstream ss;
                ss << "Parent successfully waited for Child process=" << pid[i] << " status=" << status;
                T_LOG(ss.str());
                pid[i] = 0;
            }
        }
    }
    catch( std::exception &e)
    {
        T_LOG(e.what());
    }

    // kill any child processes that have not finished.
    for( unsigned int i = 0; i < NUM_PROC; i++)
    {
        if( pid[i] != 0)
        {
            if( kill( pid[i], SIGKILL ) )
            {
                string s = "kill() failed; errno=";
                s.append(strerror(errno));
                T_LOG( s.c_str() );
            }
        }
    }

    // clean up named semaphore
    if( sem )
    {
        sem_close (sem);
        sem_unlink( SEM_NAME );  // destroys persistent semaphore
    }
    T_END;
}

int runChildProcesses()
{
    T_START;
    /* Open the pre-existing semaphore. */
    sem_t *sem = sem_open(SEM_NAME, 0);
    if( sem == SEM_FAILED )
    {
        string s="sem_open failed; errno=";
        s.append(strerror(errno));
        T_LOG( s.c_str() );
        exit(1);
    }
    else
    {
        stringstream ss;
        ss << "Semaphore=" << sem;
        T_LOG(ss.str());
    }
    sem_wait( sem );
    T_LOG("got Semaphore mySem" );
    sleep(3);

    T_LOG("post Semaphore mySem" );
    sem_post(sem);
    sem_close( sem );

    T_END;
    exit(0);
}

void syncPosixThreadsWithUnnamedPosixSemaphore()
{
    T_START;
    pthread_t* threadPtr[NUM_THREAD];
    pthread_attr_t* attr = NULL;     // use default attributes.

    // must be heap, global or shared mem, but not stack.
    sem_t* sem = (sem_t*) malloc( sizeof(sem_t));

    try
    {
        if( sem_init( sem, 0, 1 ) )
        {
            T_LOG("sem_init() failed");
            throw std::runtime_error("sem_init() failed");
        }

        // create NUM_THREAD threads
        for( unsigned int i = 0; i < NUM_THREAD; i++ )
        {
            threadPtr[i] = (pthread_t*) malloc( sizeof(pthread_t) );
        }

        int retVal = 0;

        // pass threads the semaphore address.
        void* threadActionParam = (void*) sem;

        unsigned int numThread = NUM_THREAD;
        while( numThread-- )
        {
            if( (retVal = pthread_create( threadPtr[numThread], attr, threadAction,
                                    threadActionParam )) != 0 )
            {
                stringstream strm;
                strm << "pthread_create(): retval = " << retVal;
                T_LOG(strm.str());
                throw std::runtime_error(strm.str());
            }
        }

        // wait for threads with pthread_join()
        for( unsigned int i = 0; i < NUM_THREAD; i++ )
        {
            if( threadPtr[i] )
            {
                void* ptr = NULL;
                if( ( pthread_join( *threadPtr[i], &ptr )))
                {
                    string s = "pthread_join() failed";
                    s.append(strerror(errno));
                    T_LOG( s.c_str() );
                }
                free( threadPtr[i] );
                threadPtr[i] = NULL;
            }
        }
        sem_destroy( sem );
        free( sem );
        sem = 0;

    }
    catch( std::exception &e )
    {
        T_LOG( e.what() );
    }

    // cleanup
    pthread_attr_destroy( attr );

    for( unsigned int i = 0; i < NUM_THREAD; i++ )
    {
        if( threadPtr[i] )
        {
            pthread_cancel( *threadPtr[i] );
            free( threadPtr[i] );
        }
    }
    if( sem )
    {
        sem_destroy( sem );
        free( sem );
    }
    T_END;
}

static void* threadAction( void* parameters )
{
    T_START;

    // print out the thread id
    pthread_t myThread = pthread_self();
    std::stringstream s1 = std::stringstream();
    s1 << "threadId = " << myThread << endl;
    T_LOG(s1.str());

    sem_t* sem = (sem_t*) parameters;
    sem_wait( sem );
    T_LOG("got unnamed Semaphore" );

    sleep(3);
    sem_post( sem );
    T_LOG("post unnamed Semaphore" );

    T_END;
    void* ptr = 0;// pointer to return information. Unused for now.
    return ptr;
}
