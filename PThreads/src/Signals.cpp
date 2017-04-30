/*
 * Signals.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: lburley
 *      Description: 1. signal handling of SIGINT signal. see signalHandling()
 *                   2. signal handling using SIGALARM and busy wait. see startAlarmSetBusyWait()
 *                   3. signal handling using blocking POSIX semaphore. See startAlarmSem()
 *                      References: See sem_init(), sem_wait(), sem_post(),  sem_destroy().
 *                      Unused:  sem_getValue();
 *                   4. todo : use sigmask()..
 *
 *                   http://en.cppreference.com/w/cpp/utility/program/sig_atomic_t
 *
 *                  <semaphore.h> defines POSIX semaphores, which are designed in such a way that they can be implemented
 *                  entirely in userspace.
 *                  POSIX semaphores also offer you the choice of whether you want a process-local semaphore
 *                  (for use in a multi-threaded environment, or even, in some conditions, a signal handler in a
 *                  single-threaded program) or a process-shared one, and
 *                  in the latter case, you also have the choice whether to let the system handle allocating it in a
 *                  shared namespace by name, or to obtain shared memory yourself and initialize it in shared memory.
 *                  ( See http://stackoverflow.com/questions/11058045/differences-between-semaphore-h-and-sys-sem-h )
 *                  See sem_post(), sem_wait(), sem_init().
 */

#include <unistd.h>
#include <semaphore.h>

#include <csignal>
#include <climits>
#include <cassert>
#include <ctime>

#include <cstdio>
#include <cstdlib>

using namespace std;

// utility functions
static void err_sys( const char* );
static void startAlarm( unsigned int );

// SIGINT functions
static void sigInt( int );

// busy wait alarm handling
static void sigAlarmBusyWait( int );
static void startAlarmSetBusyWait();

// semaphore blocking alarm handling
static void sigAlarmSem( int );
static void startAlarmSetSem();

// http://en.cppreference.com/w/cpp/utility/program/sig_atomic_t
// http://stackoverflow.com/questions/24931456/how-sig-atomic-t-actually-works
static volatile sig_atomic_t num_alarms = 0;
static sem_t semAlarm;

#define MAX_ALARMS 5

void signalHandling()
{

    printf( "Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__ );

    // set up signal handling; if received a kill -2 pid, or a CTRL C;
    // then SIGINT occurs, handle with sig_int() routine;
    if( signal( SIGINT, sigInt ) == SIG_ERR )
    {
        err_sys( "SIGINT signal error" );
    }

    startAlarmSetBusyWait();
    startAlarmSetSem();

    printf( "End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__ );
}

void err_sys( const char* x )
{
    // print to stderr stream
    perror( x );
    exit( 1 );
}

void sigInt( int signo )
{
    static int n = 0;
    assert( signo == SIGINT );
    printf( "SIGINT Received!! Count=%d Hit me again!!\n", n );
    if( ++n >= 5 )
    {
        printf( "exiting!!|n" );
        exit( 0 );
    }
}

void sigAlarmBusyWait( int signo )
{
    assert( signo == SIGALRM );
    num_alarms++;
    printf( "SIGALRM Received!! num_alarms=%d\n", num_alarms );
}

// start of busy waiting alarm handling code
void startAlarmSetBusyWait()
{
    int oldAlarmCount = 0;

    // set up signal handling for Alarms
    // when SIGALRM occurs, handle with sigAlarmBusyWait() routine;
    if( signal( SIGALRM, sigAlarmBusyWait ) == SIG_ERR )
    {
        err_sys( "SIGALRM signal error" );
    }

    startAlarm( 1 );
    while( num_alarms < MAX_ALARMS )   // poll and busy wait
    {
        if( oldAlarmCount < num_alarms )
        {
            oldAlarmCount = num_alarms;
            startAlarm( 1 );
        }
    }
    alarm(0);   // cancel alarm that may be left..
    return;
}

void startAlarm( unsigned int seconds )
{
    time_t timeSpec;
    time( &timeSpec );

    struct tm timeInfo;
    //  timeInfo = localtime( &timeSpec );
    localtime_r( &timeSpec, &timeInfo );    // non-portable, but thread-safe version
    printf( " starting alarm for %d seconds: %s", seconds, asctime( &timeInfo ) );
    if( alarm( seconds ) != 0 )
    {
        // outstanding alarm still there. Race condition??
        printf("error\n");
    }
}


// start of semaphore-related alarm handling code

void sigAlarmSem( int signo )
{
    assert( signo == SIGALRM );
    num_alarms++;
    printf( "SIGALRM Received!! Invoking Signalling semaphore;  num_alarms=%d\n", num_alarms );
    sem_post( &semAlarm );
    return;
}


// change signal handling for Alarms to use semaphores
void startAlarmSetSem()
{
    int numSemAlarms = 0;

    // when SIGALM occurs, handle with sig_int() routine;
    if( signal( SIGALRM, sigAlarmSem ) == SIG_ERR )
    {
        err_sys( "SIGALRM sigAlarmSem() signal error" );
    }

    // initialize the unamed semaphore; it is local to this process and shared by threads.
    sem_init( &semAlarm, 0, 0 );

    while( numSemAlarms < MAX_ALARMS )
    {
        startAlarm( 2 );
        // block on alarm semaphore
        int numAttempts = 0;
        while( true )
        {
            // check return value and retry if not successful.
            // Sometimes we get "interrupted System Call result; so just retry...
            if( sem_wait( &semAlarm ) == 0 ) break;
            numAttempts++;
            if( numAttempts > 1000 ) break;
        }
        printf("numAttempts = %d\n", numAttempts);
        numSemAlarms++;
    }
    alarm(0);                  // clean up
    sem_destroy( &semAlarm );  // clean up
}

