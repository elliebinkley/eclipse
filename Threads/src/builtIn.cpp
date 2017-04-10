/*
 * builtIn.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Burley
 *      Description: The GNU compiler provides some built-ins for atomic operations.
 *      Test operations with and without these atomics to see if they make a difference.
 *      See https://en.wikipedia.org/wiki/Memory_barrier
 *          https://en.wikipedia.org/wiki/Volatile_(computer_programming)#Example_of_memory-mapped_I.2FO_in_C
 *          https://gcc.gnu.org/onlinedocs/gcc/_005f_005fsync-Builtins.html#g_t_005f_005fsync-Builtins
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

#include <SharedLibrary/inc/MyLogger.hpp>

#define NUM_THREADS 400
#define COUNT 10000

constexpr unsigned int TOTAL = NUM_THREADS * COUNT;

static volatile unsigned int x = 0;
static volatile unsigned int y = 0;
static volatile unsigned long long int p = 0;
static volatile unsigned long long int q = 0;

static void* threadIncrement( void* );

using namespace std;
void builtIn()
{
    T_START

    stringstream ss;
    ss << "size of int=" << sizeof(int) << "  sizeof(long long)=" << sizeof( long long);
    T_LOG( ss.str() )

    // create child thread with the attributes ( &attr) of the parent.
    // thread starts running at threadIncrement().
    int retVal = 0;

    pthread_attr_t attr;
    if( (retVal = pthread_attr_init( &attr )) != 0 )
    {
        stringstream text;

        text << "pthread_attr_init() failed; exiting:" << retVal;
        T_LOG( text.str() );
        pthread_exit( NULL );
    }

    /*
     ret = __getcpu(&cpu, NULL, NULL);

     sched_getcpu();
     sched_getaffinity
     pthread_setaffinity_np;
     */
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
        } else
        {
            std::stringstream s;
            s << "thread creation failed; retVal=" << retVal;
            T_LOG( s.str() );
        }
    }

    // no need for the data structure now.
    pthread_attr_destroy( &attr );

    // int pthread_join(pthread_t thread, void **value_ptr);
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
    assert(TOTAL == x);

    s.clear();
    s.str( "" );
    s << "y=" << y << "  expected y=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL != y); // with the number of threads and iterations, lack of thread safety is revealed

    s.clear();
    s.str( "" );
    s << "p=" << p << "  expected p=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL == p );

    s.clear();
    s.str( "" );
    s << "q=" << q << "  expected q=" << COUNT * NUM_THREADS;
    T_LOG( s.str() )
    assert(TOTAL != q );  // with the number of threads and iterations, lack of thread safety is revealed
    T_END;
}

// increments four counters.
void* threadIncrement( void* data )
{
    for( unsigned int i = 0; i < COUNT; i++ )
    {
        // test with integers
        __sync_add_and_fetch( &x, 1 );   //     thread-safe
        y = y + 1;                       // not thread-safe
        // test with long long int
        __sync_add_and_fetch( &p, 1 );   //     thread-safe
        q = q + 1;                       // not thread-safe
    }

    std::string *termStatus = new std::string( "Done" );
    pthread_exit( (void*) termStatus );
    return ( NULL);
}
