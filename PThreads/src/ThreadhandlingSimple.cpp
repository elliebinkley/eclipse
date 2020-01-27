//============================================================================
// Name        : ThreadsHandling.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : The static array myTestInfo gets statically initialized with NUM_THREADS TestInfo objects prior to main()
//             : main() creates a bunch of threads. Each thread adds a random number to a global sum value and waits 5 seconds.
//             : The updating and the waiting are mutex protected. Then the thread exists.
//             : The main thread waits for each thread to complete, then when all have completed, prints out the global sum.
//
//             : Details:
//             : This program plays around with POSIX threads via calls to pthread_create();
//             : It uses mutext to protect a variable which is accessed by many threads to sum up a series of numbers.
//             : See https://computing.llnl.gov/tutorials/pthreads
//             : Note 1: Printing is done via std::stringstream so that the string is formatted completely, then
//             : streamed to cout.  This prevents strings from being interspersed between threads by the
//             : "<<" operator not guaranteeing that two strings appended by "<<" will not be interspersed
//             : " with other strings from another thread.
//             :
//             : Note: In Cygwin, threads cannot be pinned to a core via the cpuset data structure passed to
//             : the pthread_set_affinity call.
//             :
//             : See pthread_create(), pthread_join(), pthread_attr_init(),pthread_attr_destroy(),
//             : pthread_self(), pthread_getspecific(), pthread_setspecific(),
//             : pthread_exit(), pthread_key_create(), pthread_once_t(),
//             : pthread_attr_getdetachstate(), pthread_attr_getguardsize, pthread_attr_getschedparam(),
//             : thread_attr_getschedpolicy(), pthread_attr_getinheritsched(), pthread_attr_getinheritsched(),
//             : pthread_attr_getstacksize()
//             : See pthread_once_t, pthread_t, pthread_attr_t, pthread_key_t
//             :
//============================================================================

#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>

#include <ctime>
#include <cassert>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <array>
#include <cstdlib>
#include <vector>

#include <UtilitiesLib/inc/MyLogger.hpp>

using namespace std;

static void* doSomething( void* param );
static void printThreadAttributes( pthread_attr_t attr );
static void threadInit( void );
static void threadDestroy( void * );
static void printSum( void );

// Thread tracking object
static const unsigned int NUM_THREADS = 500;
class TestInfo
{
public:
    pthread_t myThreadInfo;          // thread info for thread
    pthread_t mainThreadInfoValue;   // thread info for main thead.
    long int randomValue;            // a random number which will be part of a global sum
                                     // contained in global variable randomSum
    TestInfo() :
            myThreadInfo( 0 ), mainThreadInfoValue( 0 ), randomValue( rand() / NUM_THREADS )
    {
        cout << "TestInfo:ctor(): randomValue=" << randomValue << endl;
    }
    std::string toString()
    {
        std::stringstream s = std::stringstream();
        s << "myThreadInfoPtr=" << myThreadInfo << " mainThreadInfoValue=" << mainThreadInfoValue
                << endl;
        std::string str = s.str();
        return str;
    }
};

// Static class thread tracking static array object
typedef std::array<TestInfo, NUM_THREADS> MyTestInfo;
static MyTestInfo myTestInfo { };  // initializes with default constructor

// vector for tracking pthread_key_t keys
static std::vector<pthread_key_t> *threadKeys = 0;

static int randomSum = 0;                // sum of all pthread randomValues from TestInfo
static pthread_mutex_t* randomSumMutex;  // mutex to protect randomSum

void posixThreadHandlingWithPosixMutex( void )
{

    T_START;

    unsigned int retVal = 0;
    // print thread attributes
    pthread_attr_t attr;
    if( (retVal = pthread_attr_init( &attr )) != 0 )
    {
        cout << "pthread_attr_init() failed; exiting:" << retVal << endl;
        pthread_exit( NULL );
    }
    printThreadAttributes( attr );

    // print out the main thread id
    pthread_t myThread = pthread_self();
    std::stringstream s1 = std::stringstream();
    s1 << "\n main threadId = " << myThread << endl;
    std::cout << s1.str();

    // initialize threadinit() to initialize setup
    pthread_once_t once = PTHREAD_ONCE_INIT;
    if( (retVal = pthread_once( &once, threadInit )) != 0 )
    {
        cout << "pthread_once() failed; exiting" << retVal << endl;
        pthread_exit( NULL );
    }

    // create threads
    MyTestInfo::iterator iter;
    for( iter = myTestInfo.begin(); iter < myTestInfo.end(); iter++ )
    {
        // store parent thread id
        iter->mainThreadInfoValue = myThread;

        // create child thread with the attributes ( &attr) of the parent.
        // thread starts running at doSomething.
        // Pass the doSomething to the iter as an argument.
        retVal = pthread_create( &iter->myThreadInfo, &attr, doSomething, (void*) iter );
        if( retVal == 0 )
        {
            std::stringstream s;
            s << "thread creation succeeded; threadId=" << iter->myThreadInfo << endl;
            std::cout << s.str() << endl;
        } else
        {
            std::stringstream s;
            s << "thread creation failed; retVal=" << retVal << endl;
            std::cout << s.str();
        }
    }

    // no need for the data structure now.
    pthread_attr_destroy( &attr );

    // wait for each thread to complete...
    for( iter = myTestInfo.begin(); iter < myTestInfo.end(); iter++ )
    {
        std::string* res;
        retVal = pthread_join( iter->myThreadInfo, (void**) &res );
        if( !retVal )
        {
            std::stringstream sDone;
            sDone << "Joined with thread=" << iter->myThreadInfo << ":retVal= " << res->c_str();
            cout << sDone.str() << endl;
            delete (res);
        } else cout << "bad result=" << retVal << endl;
    }

    // destroy main thread data structures.
    if( threadKeys ) delete threadKeys;

    printSum();

    if( ( retVal = pthread_mutex_destroy(randomSumMutex)) != 0 )
    {
        cout << "pthread_mutex_destroy() failed=" << retVal << endl;
    }
    if( randomSumMutex ) delete randomSumMutex;

    T_END;
    return;
}

static void* doSomething( void* data )
{
    int retVal = 0;
    pthread_t id = ((TestInfo*) (data))->myThreadInfo;

    std::stringstream s;
    s << "Start:  Function=" << __FUNCTION__ << " File=" << __FILE__ << " Line=" << __LINE__
            << " threadID=" << id;
    std::cout << s.str() << endl;

    // return string info;  the main thread will destroy the memory when it joins.
    std::stringstream retStatus;
    retStatus << "Thread id" << id << " terminating" << endl;
    std::string *termStatus = new std::string( retStatus.str() );

    if( pthread_self() != id )
    {
        std::stringstream errormsg;
        errormsg << "thread ID mismatch; myThread=" << pthread_self() << " passed threadID=" << id
                << endl;
        pthread_exit( (void*) termStatus );
    }

    // get the thread key from the vector and create an associated thread specific string
    pthread_key_t key = threadKeys->front();
    if( key == NULL )
    {
        cout << "key is NULL << endl";
        pthread_exit( (void*) termStatus );
    }

    // check if the thread key is associated with a value for this thread; it should not be.
    void* ptr;
    if( (ptr = pthread_getspecific( key )) != NULL )
    {
        // error; key with a valid value should not exist yet..
        cout << "error:key already has a value " << endl;
        pthread_exit( (void*) termStatus );
    }

    // associate the key with a thread specific string.
    // once the key is associated, then threadDestroy() will be called with the associated data passed to threadDestroy
    std::stringstream temp;
    temp << "thread=" << id << ";threadkey=" << key << endl;
    if( (retVal = pthread_setspecific( key, new std::string( temp.str() ) )) != 0 )
    {
        cout << "error:pthread_setspecific(key) failed " << retVal << endl;
        pthread_exit( (void*) termStatus );
    }

    // use try/catch so that p_thread_exit is always called. Thus, threadDestroy() will always be called.
    // Thus the mutex will always be freed in threadDestroy().
    try
    {
        // update the global sum; protect with mutex

        // if fails, might be because this thread already owns the mutex...
        if( (retVal = pthread_mutex_lock( randomSumMutex )) != 0 )
        {
            stringstream ss;
            ss << "pthread_mutex_lock() failed=" << retVal << endl;
            throw ss.str();
        }

        randomSum = randomSum + ((TestInfo*) (data))->randomValue;

        // print data
        /*
         std::stringstream s5 = std::stringstream();
         s5 << "\nadding randomValue=" << ((TestInfo*) (data))->randomValue << " to randomSum="
         << randomSumOld << " to get new randomSum=" << randomSum;
         std::cout << s5.str() << endl;
         */
        //   sleep( 5 );
        // if fails, might be because the mutex is not locked by this thread...
        if( (retVal = pthread_mutex_unlock( randomSumMutex )) != 0 )
        {
            stringstream ss;
            ss << "pthread_mutex_unlock() failed=" << retVal << endl;
            throw ss.str();
        }
    }
    catch( std::exception &e )
    {
        stringstream ss;
        ss << "caught exception =" << e.what();
        T_LOG( ss.str() );
    }
    T_END;
    pthread_exit ( (void*) termStatus );
    return ( NULL);
}

// print out thread attributes available in Cygwin:
// thread attributes are contained in "attr".
static void printThreadAttributes( pthread_attr_t attr )
{

    cout << "print attributes of threads as implemented in Cygwin" << endl;

    int attr_value_int = 0;
    // thread state
    if( !pthread_attr_getdetachstate( &attr, &attr_value_int ) )
    {
        cout << "detachState=" << attr_value_int
                << (attr_value_int ? ":PTHREAD_CREATE_DETACHED" : ":PTHREAD_CREATE_JOINABLE")
                << endl;
    }

    // guard-size; e.g. buffer area for stack protection
    size_t attr_value_size_t = 0;
    if( !pthread_attr_getguardsize( &attr, &attr_value_size_t ) )
    {
        cout << "guardSize=" << attr_value_size_t << endl;
    }

    // scheduler
    if( !pthread_attr_getinheritsched( &attr, &attr_value_int ) )
    {
        cout << "scheduler=" << attr_value_int
                << (attr_value_int ? ":PTHREAD_EXPLICIT_SCHED" : ":PTHREAD_INHERIT_SCHED") << endl;
    }

    // sched parameters; e.g. priority
    struct sched_param sched;
    if( !pthread_attr_getschedparam( &attr, &sched ) )
    {
        cout << "scheduler priority=" << sched.sched_priority << endl;
    }

    // scheduler policy;
    if( !pthread_attr_getschedpolicy( &attr, &attr_value_int ) )
    {
        std::array<std::string, 3> policy { ":PTHREAD_PRIO_NONE", ":PTHREAD_PRIO_INHERIT",
                ":PTHREAD_PRIO_PROTECT" };
        cout << "schedPolicy=" << attr_value_int << policy[attr_value_int] << endl;
    }

    // thread scope;
    if( !pthread_attr_getscope( &attr, &attr_value_int ) )
    {
        std::array<std::string, 2> scope { ":PTHREAD_SCOPE_PROCESS", ":PTHREAD_SCOPE_SYSTEM" };
        cout << "thread scope=" << attr_value_int << scope[attr_value_int] << endl;
    }

    // stack size;
    unsigned long attr_value_long;
    if( !pthread_attr_getstacksize( &attr, &attr_value_long ) )
    {
        cout << "stack size=" << attr_value_long << endl;
    }
}

// set up via pthread_once();
static void threadInit( void )
{
    // initialize thread Key vector
    assert( !threadKeys );
    if( !threadKeys )
    {
        threadKeys = new std::vector<pthread_key_t>;
    }

    // Create a key and associate it with a destructor routine; threadDestroy()
    // On threadExit, thread_destroy() will be called. This enables thread specific cleanup of owned
    // mutexes, allocated memory and other resources.
    pthread_key_t threadKey_Destruct;
    int retVal = 0;
    if( (retVal = pthread_key_create( &threadKey_Destruct, threadDestroy )) != 0 )
    {
        cout << "pthread_key_create() failed; exiting:" << retVal << endl;
        pthread_exit( NULL );
    }

    // Put this key in a vector of keys; note the vector will only have one element.
    threadKeys->push_back( threadKey_Destruct );

    // The key can have thread specific information. This information will be passed to threadDestroy()
    // when this thread is terminated.
    std::stringstream temp;
    pthread_t mainThread = pthread_self();
    temp << "thread=" << mainThread << ";threadkey=" << threadKey_Destruct << " File=" << __FILE__
            << endl;
    if( (retVal = pthread_setspecific( threadKey_Destruct, new std::string( temp.str() ) )) != 0 )
    {
        cout << "thread_setspecific(threadKey_Destruct) failed:" << retVal << endl;
        pthread_exit( NULL );
    }

    // create and initialize a mutex to be used to protect variable randomSum.
    // Upon successful initialization, the state of the mutex becomes initialized and unlocked
    // There are two types of attributes: type ( with a default of PTHREAD_MUTEX_NORMAL and
    // robust( with a default of PTHREAD_MUTEX_ROBUST)

    pthread_mutexattr_t mutex_attr;
    if( (retVal = pthread_mutexattr_init( &mutex_attr )) != 0 )
    {
        cout << "pthread_mutexattr_init() failed:" << retVal << endl;
        pthread_exit( NULL );
    }

    // protect against deadlock with PTHREAD_MUTEX_ERRORCHECK
    if( (retVal = pthread_mutexattr_settype( &mutex_attr, PTHREAD_MUTEX_ERRORCHECK )) != 0 )
    {
        cout << "pthread_mutexattr_init() failed:" << retVal << endl;
        pthread_exit( NULL );
    }

    // set ROBUST mutex
    /* Not in Cygwin...
     if( (retVal =  pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_ROBUST)) != 0 )
     {
     cout << "pthread_mutexattr_setrobust() failed:" << retVal << endl;
     pthread_exit( NULL );
     }
     */

    // set priority of thread holding the mutex
    /* Not in Cygwin...
     int x = 0;
     if( (retVal =  pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_NONE)) != 0 )
     {
     cout << "pthread_mutexattr_getprotocol() failed:" << retVal << endl;
     pthread_exit( NULL );
     }
     */

    randomSumMutex = new pthread_mutex_t();
    if( (retVal = pthread_mutex_init( randomSumMutex, &mutex_attr )) != 0 )
    {
        cout << "pthread_mutex_init failed:" << retVal << endl;
        pthread_exit( NULL );
    }

    // no need for the data structure now.
    if( (retVal = pthread_mutexattr_destroy( &mutex_attr )) != 0 )
    {
        cout << "pthread_mutexattr_destroy() failed:" << retVal << endl;
        pthread_exit( NULL );
    }

    cout << "threadInit() done" << endl;
    return;
}

// set up as part of pthread_create_key(); called on pthread_exit()
static void threadDestroy( void* ptr )
{
    if( ptr != NULL )
    {
        // ptr should be a std::string pointer set up with pthread_key_create()
        std::string* strPtr = (std::string*) ptr;
        std::stringstream temp;
        temp << "threadDestroy() strPtr=" << strPtr->c_str();
        cout << temp.str() << endl;
        delete strPtr;
    } else
    {
        cout << "threadDestroy() ptr == NULL" << endl;
    }

    // we don't want the thread to exit if it still owns the mutex, so free the mutex.
    // pthread_mutex_unlock() fails if the current thread does not own the mutex
    pthread_mutex_unlock( randomSumMutex );
    return;
}

static void printSum()
{
    long int sum = 0;
    MyTestInfo::const_iterator iter;

    for( iter = myTestInfo.begin(); iter != myTestInfo.end(); iter++ )
    {
        sum = sum + iter->randomValue;
    }

    pthread_mutex_lock( randomSumMutex );
    const long int myRandomSum = randomSum;
    pthread_mutex_unlock( randomSumMutex );
    assert( sum == myRandomSum );
    cout << "Success: sum=" << sum << " :randomSum=" << myRandomSum << endl;
}

