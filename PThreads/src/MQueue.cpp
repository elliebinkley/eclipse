/*
 * MQueue.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: Larry Burley
 *      Description:  create a bunch of threads senders and receivers and connect them with a POSIX message queue.
 *      Todo: 1. when cancelthread, register handlers...
 *            2. implement on processes
 *            3. run with different random combinations.
 */

#include <unistd.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <pthread.h>
#include <mqueue.h>
#include <string>
#include <iostream>
#include <atomic>


#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;

const unsigned int NUM_SENDERS = 200;
const unsigned int NUM_MSG_PER_SENDER = 1000;
const unsigned int NUM_RECEIVERS = 300;
const unsigned int MSG_SIZE = 4096;
const unsigned int MSG_MAX = 500;
constexpr unsigned int NUM_MSGS = ( NUM_SENDERS * NUM_MSG_PER_SENDER ) + 5;
const char* myQueueName1="/myQueueName1";
mqd_t mQ1;

static volatile std::atomic<unsigned int> numSent;
static volatile std::atomic<unsigned int> numReceived;
static volatile std::atomic<unsigned int> numErrors;

static void* doReceiveThreads( void* params );
static void* doSendThreads( void* params );
static void* doReceiveProcesses( void* params );
static void* doSendProcesses( void* params );
static void posixThreadsWithPosixMessageQueues( mq_attr* attr );
static void posixProcessesWithPosixMessageQueues( mq_attr* attr );
static void startThreads( pthread_t** threadPtrPtr, unsigned int size, void* function, mq_attr* attr, bool send );
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
    mqd_t mQ1 = mq_open( myQueueName1, O_RDWR | O_CREAT | O_EXCL,  0664, &attr);
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

    mq_close( mQ1 );
    mq_unlink( myQueueName1 );  // deletes the queue.
    T_END;
}

void posixThreadsWithPosixMessageQueues(mq_attr* attr)
{
    T_START;
    numSent = 0;
    numReceived = 0;
    numErrors = 0;
    int numLeft = 0;
    pthread_t* threadPtrSenders[NUM_SENDERS];
    pthread_t* threadPtrReceivers[NUM_RECEIVERS];
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
    cleanupThreads( threadPtrSenders, NUM_SENDERS );
    cleanupThreads( threadPtrReceivers, NUM_RECEIVERS );
    stringstream s;
    s << "numSent=" << numSent << " numReceived=" <<  numReceived << " numErrors=" << numErrors << endl;
    T_LOG( s.str().c_str() );
    assert( numSent == numReceived );
    assert( numSent == ( NUM_MSG_PER_SENDER * NUM_SENDERS) );
    assert(!numErrors);
    T_END;
}

void posixProcessesWithPosixMessageQueues(mq_attr* attr)
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

    mqd_t mq = *( ( mqd_t* )mqPtr );
    char buf[ MSG_SIZE ];
    unsigned int msgPriority;
    int numThreadReceived = 0;
    while( true )
    {
        buf[0] = 0;
        ssize_t numBytes = mq_receive(mq, buf, MSG_SIZE, &msgPriority );
        assert( numBytes < MSG_SIZE );

        numReceived++;
        numThreadReceived++;
/*
        stringstream s;
        s <<"message#" << numReceived << "=" << buf << endl;;
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
 //   T_START;

    int numMessagePerThread = NUM_MSG_PER_SENDER;
    mqd_t mq = *( ( mqd_t* )mqPtr );
    unsigned int numThreadSent = 0;

    char* packet = "message!!";
    int sz = strlen(packet);
    unsigned int priority = 3;

    while( numMessagePerThread-- )
    {
        if( mq_send( mq, packet, sz, priority ))
        {
            stringstream s;
            s << "mq_send() failed; perror=" << strerror(errno);
            numErrors++;
            T_LOG(s.str().c_str());
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

void startThreads( pthread_t** threadPtrPtr, unsigned int size, void* function, mq_attr* mqAttr, bool send )
{
    T_START;
    int oFlags = 0;

    if( send ) oFlags = O_WRONLY;
    else       oFlags = O_RDONLY;
    mQ1 = mq_open( myQueueName1, oFlags,  0664, mqAttr);

    pthread_attr_t* attr = NULL;     // use default attributes.
    for( unsigned int i = 0; i < size; i++ )
    {
        *threadPtrPtr = (pthread_t*) malloc( sizeof(pthread_t) );
        void* ptr = NULL;
        if( pthread_create( *threadPtrPtr, attr, (void* (*)( void* ) )function, &mQ1 ))
        {
            std::string s = "pthread_create() failed";
            s.append(strerror(errno));
            T_LOG(s.c_str());
        }
        threadPtrPtr++;
    }
    T_END;
}

void waitForThreads(pthread_t** threadPtrPtr, unsigned int size )
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
            }
            free( *threadPtrPtr );
            *threadPtrPtr = NULL;
        }
        threadPtrPtr++;
    }
    T_END;
}

static void cleanupThreads(pthread_t** threadPtrPtr, unsigned int size)
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
           pthread_cancel( **threadPtrPtr );
           free( *threadPtrPtr );
           *threadPtrPtr = NULL;
       }
       threadPtrPtr++;
   }
   T_END;
}

// poll the Q poll times, and if not empty log a message.
// Return number of messages in the Q.
unsigned int checkQEmpty(unsigned int poll )
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



