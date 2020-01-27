/*
 * MQueue.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: Larry Burley
 *      Description: 1. Create a bunch of threads senders and receivers and connect them with a POSIX message queue.
 *                   2. Use a condition variable to start sending all at once.
 *                   3. typical producer/consumer implemented using POSIX queues.
 *                   4. todo: use mq_notify() and use a manager to parcel out messages when notified that the queue is not empty.
 *                   5. todo: use mq_timedreceive(), mq_timedsend().
 *                   6. mq_thread_join() can wait forever if it waits for a thread that is hung.
 *                      This program is vulnerable to that scenario.
 *      Todo: 1. implement on  a process basis.
 *            2. run with different random combinations for performance metrics. Different number of threads for producer/consumer.
 *      References:
 *            https://users.pja.edu.pl/~jms/qnx/help/watcom/clibref/mq_overview.html#MqFlags
 */

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <pthread.h>
#include <mqueue.h>

#include <string>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <atomic>
#include <sstream>

#include <UtilitiesLib/inc/MyLogger.hpp>

using namespace std;

const unsigned int NUM_SENDERS = 200;
const unsigned int NUM_MSG_PER_SENDER = 1000;
const unsigned int NUM_RECEIVERS = 300;
const unsigned int MSG_SIZE = 4096;
const unsigned int MSG_MAX = 500;
constexpr unsigned int NUM_MSGS = (NUM_SENDERS * NUM_MSG_PER_SENDER) + 5;
const char* myQueueName1 = "/myQueueName1";    // name if message queue.
const char* packet = "message!!";            // contents of each message..
mqd_t mQ1;                                   // file descriptor for message queue

static pthread_mutex_t count_mutex;                 // condition variable mutex
static pthread_cond_t count_threshold_cv;           // condition variable.
static volatile std::atomic<unsigned int> numThreadsInitialized;

static volatile std::atomic<unsigned int> numSent;
static volatile std::atomic<unsigned int> numReceived;
static volatile std::atomic<unsigned int> numErrors;

static void* doReceiveThreads( void* params );
static void* doSendThreads( void* params );
static void* doReceiveProcesses( void* params );
static void* doSendProcesses( void* params );
static void posixThreadsWithPosixMessageQueues( mq_attr* attr );
static void posixProcessesWithPosixMessageQueues( mq_attr* attr );
static void startThreads( pthread_t** threadPtrPtr, unsigned int size, void* function,
                          mq_attr* attr, bool send );
static void waitForLastThread();
static void waitForThreads( pthread_t** threadPtrPtr, unsigned int size );
static void cleanupThreads( pthread_t** threadPtrPtr, unsigned int size );
static unsigned int checkQEmpty( unsigned int poll );

void threadHandlingwithMqueue()
{
    T_START;
    struct mq_attr attr;

    attr.mq_maxmsg = MSG_MAX;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_flags = 0;

    // open Q; if it exists, then delete Q and exit
    mqd_t mQ1 = mq_open( myQueueName1, O_RDWR | O_CREAT | O_EXCL, 0664, &attr);
    if( mQ1 == -1 )
    {
        stringstream s;
        s << "mq_open() failed on " << myQueueName1 << " perror=" << strerror( errno ) << " deleting the Q under /dev/mqueue";
        T_LOG( s.str().c_str() );
        mq_unlink( myQueueName1 );
        assert(false);
    }

    posixThreadsWithPosixMessageQueues(&attr);
    posixProcessesWithPosixMessageQueues(&attr);

    if( mq_close( mQ1 ) == -1 )
    {
        stringstream s;
        s << "mq_close() failed; perror=" << strerror(errno);
        T_LOG( s.str().c_str() );
        numErrors++;
    }
    mq_unlink( myQueueName1 );  // deletes the queue.
    T_END;
}

void posixThreadsWithPosixMessageQueues( mq_attr* attr )
{
    T_START;
    numSent = 0;
    numReceived = 0;
    numErrors = 0;
    int numLeft = 0;
    pthread_t* threadPtrSenders[NUM_SENDERS];
    pthread_t* threadPtrReceivers[NUM_RECEIVERS];

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init (&count_threshold_cv, NULL);
    numThreadsInitialized=0;

    try
    {
        startThreads(threadPtrReceivers, NUM_RECEIVERS, (void *)doReceiveThreads, attr, false);
        startThreads(threadPtrSenders, NUM_SENDERS, (void *)doSendThreads, attr, true );
        waitForThreads(threadPtrSenders, NUM_SENDERS );
        if( (numLeft = checkQEmpty( 20 )) != 0 )
        {
            assert(!numLeft);
        }
        cleanupThreads( threadPtrReceivers, NUM_RECEIVERS );
    }
    catch ( std::exception &e )
    {
        T_LOG( e.what() );
    }
    T_LOG("General Cleanup");
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);

    cleanupThreads( threadPtrSenders, NUM_SENDERS );
    cleanupThreads( threadPtrReceivers, NUM_RECEIVERS );

    // accounting....
    stringstream s;
    s << "numSent=" << numSent << " numReceived=" << numReceived << " numErrors=" << numErrors << endl;
    T_LOG( s.str().c_str() );
    assert( numSent == numReceived );
    assert( numSent == ( NUM_MSG_PER_SENDER * NUM_SENDERS) );
    assert( !numErrors );
    T_END;
}

void posixProcessesWithPosixMessageQueues( mq_attr* attr )
{
    T_START;
    try
    {

    }
    catch ( std::exception &e)
    {
        T_LOG( e.what() );
    }
    T_END;
}

void* doReceiveThreads( void* mqPtr )
{
    //   T_START;

    mqd_t mq = *((mqd_t*) mqPtr);
    char buf[MSG_SIZE];
    unsigned int msgPriority;
    int numThreadReceived = 0;
    while( true )
    {
        buf[0] = 0;
        ssize_t numBytes = mq_receive( mq, buf, MSG_SIZE, &msgPriority );
        if( numBytes == -1 )
        {
            stringstream s;
            s << "mq_receive() failed; perror=" << strerror( errno );
            T_LOG( s.str().c_str() );
            numErrors++;
        }
        assert( numBytes < MSG_SIZE );

        numReceived++;
        numThreadReceived++;

        /*      Debug
         stringstream s;
         s <<"message#" << numReceived << "=" << buf << endl;
         T_LOG(s.str().c_str() );
         */
//        sleep(1);
    }

//    T_END;
    void* ret = NULL;
    return ret;
}

void* doSendThreads( void* mqPtr )
{
//    T_START;

    int numMessagePerThread = NUM_MSG_PER_SENDER;
    mqd_t mq = *((mqd_t*) mqPtr);

    int sz = strlen( packet );
    unsigned int priority = 3;

    // wait for last thread to be created before message sending starts.
    waitForLastThread();

    // start sending messages.
    unsigned int numThreadSent = 0;
    while( numMessagePerThread-- )
    {
        if( mq_send( mq, packet, sz, priority ) )
        {
            stringstream s;
            s << "mq_send() failed; perror=" << strerror( errno );
            T_LOG( s.str().c_str() );
            numErrors++;
        }

        numSent++;
        numThreadSent++;
        /*
         std::stringstream s;
         s << packet << numMessagePerThread << endl;
         T_LOG( s.str().c_str() );
         */
    }

//    T_END;
    void* ret = NULL;
    return ret;
}

void* doReceiveProcesses( void* params )
{
    T_START;
    void* ret = NULL;
    T_END;
    return ret;
}

void* doSendProcesses( void* params )
{
    T_START;
    void* ret = NULL;
    T_END;
    return ret;
}

void startThreads( pthread_t** threadPtrPtr, unsigned int size, void* function, mq_attr* mqAttr,
                   bool send )
{
    T_START;
    int oFlags = 0;

    if( send ) oFlags = O_WRONLY;
    else oFlags = O_RDONLY;
    if( (mQ1 = mq_open( myQueueName1, oFlags, 0664, mqAttr)) == -1 )
    {
        std::string s = "mq_open() failed; perror=";
        s.append(strerror(errno));
        T_LOG(s.c_str());
        numErrors++;;
    }

    pthread_attr_t attr;     // use default attributes.
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for( unsigned int i = 0; i < size; i++ )
    {
        *threadPtrPtr = (pthread_t*) malloc( sizeof(pthread_t) );
        if( pthread_create( *threadPtrPtr, &attr, (void* (*)( void* ) )function, &mQ1 ))
        {
            std::string s = "pthread_create() failed";
            s.append(strerror(errno));
            T_LOG(s.c_str());
            numErrors++;;
        }
        threadPtrPtr++;
    }
    pthread_attr_destroy(&attr);

    T_END;
}

void waitForThreads( pthread_t** threadPtrPtr, unsigned int size )
{
    T_START;
    // wait for sender threads with pthread_join()
    for( unsigned int i=0; i < size; i++ )
    {
        if( *threadPtrPtr )
        {
            void* ptr = NULL;
            if( pthread_join( **threadPtrPtr, &ptr ))
            {
                string s = "pthread_join() failed;errno=";
                s.append(strerror(errno));
                T_LOG( s.c_str() );
                numErrors++;
            }
            free( *threadPtrPtr );
            *threadPtrPtr = NULL;
        }
        threadPtrPtr++;
    }
    T_END;
}

static void cleanupThreads( pthread_t** threadPtrPtr, unsigned int size )
{
    T_START;
    for( unsigned int i = 0; i < size; i++ )
    {
        if( *threadPtrPtr )
        {
            /*
             stringstream s;
             s << "cancelling thread=" << *threadPtrPtr;
             T_LOG(s.str().c_str());
             */
            if( pthread_cancel( **threadPtrPtr ) )
            {
                string s = "pthread_cancel() failed;errno=";
                s.append(strerror(errno));
                T_LOG( s.c_str() );
                numErrors++;
            }
            free( *threadPtrPtr );
            *threadPtrPtr = NULL;
        }
        threadPtrPtr++;
    }
    T_END;
}

// poll the Q poll times, and if not empty log a message.
// Return number of messages in the Q.
unsigned int checkQEmpty( unsigned int poll )
{
    T_START;
    assert( poll );  // should be greater than zero

    mqd_t mQ1 = mq_open( myQueueName1, O_RDONLY);
    if( mQ1 == -1)
    {
        stringstream s;
        s << "mq_open() failed;errno=" << strerror(errno);
        T_LOG( s.str().c_str() );
        throw std::runtime_error( s.str().c_str() );
    }

    mq_attr attr;
    unsigned int numMsgs = 0;
    while( poll-- )
    {
        if( mq_getattr(mQ1, &attr) == -1 )
        {
            stringstream s;
            s << "mq_getattr() failed;errno=" << strerror(errno);
            T_LOG( s.str().c_str() );
            throw std::runtime_error( s.str().c_str() );
        }
        numMsgs = attr.mq_curmsgs;
        if( numMsgs == 0 ) break;

        if ( poll ) sleep(1);
        else
        {
            // polling done
            stringstream s;
            s << "mQ1 not empty;messages=" << numMsgs;
            T_LOG( s.str().c_str() );
        }
    }
    T_END;
    return numMsgs;
}

void waitForLastThread()
{
    if( pthread_mutex_lock( &count_mutex ) )
    {
        stringstream s;
        s << "pthread_mutex_lock() failed; perror=" << strerror( errno );
        T_LOG( s.str().c_str() );
        numErrors++;
    }
    numThreadsInitialized++;

    //  Wait until the last thread has started. Then release other threads.
    if( numThreadsInitialized >= NUM_SENDERS )
    {
        // release all waiting threads.
        // T_LOG("cond");
        if( pthread_cond_broadcast( &count_threshold_cv ) )
        {
            stringstream s;
            s << "pthread_cond_signal() failed; perror=" << strerror( errno );
            T_LOG( s.str().c_str() );
            numErrors++;
        }
    }
    else
    {
        // wait till last thread is created.
        // T_LOG("wait");
        if( pthread_cond_wait( &count_threshold_cv, &count_mutex ) )
        {
            stringstream s;
            s << "pthread_cond_wait() failed; perror=" << strerror( errno );
            T_LOG( s.str().c_str() );
            numErrors++;
        }
    }
    if( pthread_mutex_unlock( &count_mutex ) )
    {
        stringstream s;
        s << "pthread_mutex_unlock() failed; perror=" << strerror( errno );
        T_LOG( s.str().c_str() );
        numErrors++;
    }
}

