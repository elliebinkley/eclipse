/*
 * ThreadBarrier.cpp
 *
 *  Created on: May 9, 2017
 *      Author: Burley
 *      Description: use thread barrier to make a group of threads wait until X threads reach the barrier:
 *            When the Xth thread reaches the barrir, they are all released.  This is useful for synchrnoizing threads
 *            until  they all get to a certain point, in ewhcih case they are all released, e.g. sequencing initialization
 *            is a good use case.
 *            1. start up X threads and use a barrire before they are all releases.
 *
 *
 */

#include <pthread.h>
#include <atomic>
#include <sstream>
#include <unistd.h>
#include "UtilitiesLib/inc/MyLogger.hpp"
#include "../inc/PThreadUtilities.hpp"

NUMERROR_TYPE numErrors;
static const unsigned int NUM_THREADS = 3;
static const bool Debug = false;
static pthread_barrier_t barrier;              // synchronization barrier
static void* childActions( void* data );

struct ThreadInfo
{
    pthread_t id;
    unsigned int threadNum;
};

void threadBarrier()
{
    T_START;
    int retVal = 0;
    pthread_t thread[NUM_THREADS];
    ThreadInfo threadInfo[NUM_THREADS];
    pthread_attr_t attr;

    try
    {

        if( (retVal = pthread_attr_init (&attr)) )
        {
            throwARunTimeError("pthread_attr_init()", true, &numErrors, __FILE__, __LINE__, retVal );
        }
        if( (retVal = pthread_barrier_init (&barrier, NULL, NUM_THREADS)) )
        {
            throwARunTimeError("pthread_barrier_init()", true, &numErrors, __FILE__, __LINE__, retVal );
        }

        // run a bunch of threads
        for( unsigned int i = 0; i < NUM_THREADS; i++ )
        {
            threadInfo[i].threadNum = i;
            if( (retVal = pthread_create(&thread[i], &attr, childActions, &threadInfo[i] )) )
            {
                throwARunTimeError("pthread_create()", true, &numErrors, __FILE__, __LINE__, retVal );
            }
        }
        for( unsigned int i = 0; i < NUM_THREADS; i++ )
        {
            if( (retVal = pthread_join(thread[i], nullptr )) )
            {
                throwARunTimeError("pthread_join()", true, &numErrors, __FILE__, __LINE__, retVal );
            }
        }
    } catch ( std::exception &e)
    {
        T_LOG( e.what() );
    }

    pthread_attr_destroy( &attr );

    /*

     barrierWait( rptr );                // use barrier variables to synchronize; same as above; just seeing if it works..
     int pthread_create (pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);



     pthread_exit ( (void*) termStatus );

     pthread_join(
     // wait at barrier
     void barrierWait(region* const rptr )
     {
     int ret = 0;
     if( ( ret = pthread_barrier_wait( &rptr->barrier ) ) )
     {
     if( ret != PTHREAD_BARRIER_SERIAL_THREAD )
     {
     throwARunTimeError( "pthread_barrier_wait()", false, rptr, __LINE__, ret );
     }
     }
     return;
     }
     */
    T_END;
}

void* childActions( void* data )
{
    unsigned int threadNumber = ((ThreadInfo*) (data))->threadNum;
    pthread_t id = pthread_self();
    stringstream s;
    s << "ThreadID=" << id << " threadNumber=" << threadNumber;
    if( ( threadNumber + 1) ==  NUM_THREADS )
    {
        s << " sleeping 5 seconds";
        T_LOG( s.str().c_str() );
        sleep( 5 );
    }
    else
    {
        T_LOG( s.str().c_str() );
    }

    int retVal = 0;
    if( (retVal = pthread_barrier_wait( &barrier )) )
    {
        if( retVal != PTHREAD_BARRIER_SERIAL_THREAD )
        {
            throwARunTimeError( "pthread_join()", true, &numErrors, __FILE__, __LINE__, retVal );
        }
    }
    T_END;
    pthread_exit( nullptr );
    return nullptr;
}

