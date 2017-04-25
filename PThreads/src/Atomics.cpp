/*
 * builtIn.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Burley
 *      Description: The GNU compiler provides some built-ins for atomic operations and the C++11 library
 *      provides the std::atomic for atomic operations.
 *      Test operations with these 2 atomic mechanisms and without to see if they are thread safe.
 *      See https://en.wikipedia.org/wiki/Memory_barrier
 *          https://en.wikipedia.org/wiki/Volatile_(computer_programming)#Example_of_memory-mapped_I.2FO_in_C
 *          https://gcc.gnu.org/onlinedocs/gcc/_005f_005fsync-Builtins.html#g_t_005f_005fsync-Builtins
 *          http://en.cppreference.com/w/c/atomic
 *          http://en.cppreference.com/w/cpp/atomic/atomic
 *
 */

#include <pthread.h>
#include <sched.h>

#include <cstdio>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <atomic>      // C++ version


#include <SharedLibrary/inc/MyLogger.hpp>

static const unsigned int NUM_THREADS = 400;
static const unsigned int COUNT =  10000;
static constexpr unsigned int TOTAL = NUM_THREADS * COUNT;

static pthread_key_t threadKey_Destruct;

static volatile unsigned int x = 0;
static volatile unsigned int y = 0;
static volatile std::atomic<unsigned int> z;

static volatile unsigned long long int p = 0;
static volatile unsigned long long int q = 0;
static volatile std::atomic<unsigned int> r;

static void* threadIncrement( void* );
static void  threadDestroy( void* ptr );

using namespace std;
void atomicsTest()
{
    T_START

    z = 0;
    r.store(0);   // same as r = 0;

    stringstream ss;
    ss << "size of int=" << sizeof(int) << "  sizeof(long long)=" << sizeof( long long);
    T_LOG( ss.str() )

    // create attributes ( &attr)
    int retVal = 0;

    pthread_attr_t attr;
    if( (retVal = pthread_attr_init( &attr )) != 0 )
    {
        stringstream text;

        text << "pthread_attr_init() failed; exiting:" << retVal;
        T_LOG( text.str() );
        pthread_exit( NULL );
    }

    // Create a key and associate it with cleanup routine threadDestroy()
    // This enables thread specific cleanup of owned
    // mutexes, allocated memory and other resources ( of which this file has none.. ).
    // On p_threadExit(), threadDestroy() will be called if there is a specific objects
    // associated with this thread for this key. ( See pthread_setspecific() ).
    // The only thread in this program where this occurs is the main thread, as it is the only one that
    // calls pthread_setspecific().
    //
    if( (retVal = pthread_key_create( &threadKey_Destruct, threadDestroy )) != 0 )
    {
        cout << "pthread_key_create() failed; exiting:" << retVal << endl;
        pthread_exit( NULL );
    }

    // The key can have thread specific information. This information will be passed to threadDestroy()
    // when this thread is terminated.
    std::stringstream temp;
    pthread_t mainThread = pthread_self();
    temp << "thread=" << mainThread << ";threadkey=" << threadKey_Destruct  << " File="<< __FILE__ << endl;
    if( (retVal = pthread_setspecific( threadKey_Destruct, new std::string( temp.str() ) )) != 0 )
    {
        cout << "thread_setspecific(threadKey_Destruct) failed:" << retVal << endl;
        pthread_exit( NULL );
    }

    std::array<pthread_t*, NUM_THREADS> threadArray = { };
    for( auto &i : threadArray )
    {
        i = (pthread_t*) (malloc( sizeof(pthread_t) ));
        void* param = 0;
        retVal = pthread_create( i, &attr, threadIncrement, param );
        if( retVal == 0 )
        {
            std::stringstream s;
            s << "thread creation succeeded; thread=" << *i;
           // T_LOG( s.str() );
        }
        else
        {
            std::stringstream s;
            s << "thread creation failed; retVal=" << retVal;
            T_LOG( s.str() );
        }
    }

    // no need for the data structure now.
    pthread_attr_destroy( &attr );

    // wait for all child threads.
    for( auto &i : threadArray )
    {
        std::string* res;
        retVal = pthread_join( *i, (void**) &res );
        if( !retVal )
        {
            std::stringstream sDone;
            sDone << "Joined with thread=" << *i << ":retVal= " << res->c_str();
          //  T_LOG( sDone.str() );
            delete (res);
        } else
        {
            std::stringstream sDone;
            sDone << "pthread_join bad result=" << retVal << " threadInfo=" << *i;
            T_LOG( sDone.str() );
        }

    }

    for( auto &i : threadArray )
    {
        if( i ) delete i;
    }


    std::stringstream s;
    s << "x=" << x << "  expected x=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL == x); // thread safe

    s.clear();
    s.str( "" );
    s << "y=" << y << "  expected y=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL != y); // with the number of threads and iterations, lack of thread safety is revealed

    s.clear();
    s.str( "" );
    s << "z=" << z << "  expected z=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL == z); // thread safe

    s.clear();
    s.str( "" );
    s << "p=" << p << "  expected p=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL == p ); // thread safe

    s.clear();
    s.str( "" );
    s << "q=" << q << "  expected q=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL != q );  // with the number of threads and iterations, lack of thread safety is revealed

    s.clear();
    s.str( "" );
    s << "r=" << r << "  expected r=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL == r );  // thread safe
    T_END;
}

// increments counters.
void* threadIncrement( void* data )
{
    try
    {
        for( unsigned int i = 0; i < COUNT; i++ )
        {
            // test with integers
            __sync_add_and_fetch( &x, 1 );   //     thread-safe
            y++;                             // not thread-safe
            z++;                             //     thread-safe

            // test with long long int
            __sync_add_and_fetch( &p, 1 );   //     thread-safe
            q++;                            // not  thread-safe
            r++;                            //      thread-safe
        }
    }
    catch (std::exception &e)
    {
        stringstream ss;
        ss << "execption caught=" << e.what();
        T_LOG(ss.str());
    }
    std::string *termStatus = new std::string( "Done" );
    pthread_exit( (void*) termStatus );
    return ( NULL);
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
    }
    else
    {
        cout << "threadDestroy() ptr == NULL" << endl;
    }
    return;
}

