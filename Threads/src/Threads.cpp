//============================================================================
// Name        : Threads.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : This program plays around with POSIX threads via calls to pthread_create();
//             : Note1: Printing is done via std::stringstream so that the string is formatted completely, then
//             : streamed to cout.  This prevents strings from being interspersed between threads by the
//             : "<<" operator not guaranteeing that two strings appended by "<<" will not be interspersed
//             : " with other strings from another thread.
//             : Note: In Cygwin, threads cannot be pinned to a core via the cpuset data structure passed to
//             : the pthread_set_affinity call.
//             : See pthread_create(), pthread_join(), pthread_attr_init(),pthread_attr_destroy(),
//             : pthread_self(), pthread_getspecific(), pthread_setspecific(),
//             : pthread_exit(), pthread_key_create(), pthread_once_t(),
//             : pthread_attr_getdetachstate(), pthread_attr_getguardsize, pthread_attr_getschedparam(),
//             : thread_attr_getschedpolicy(), pthread_attr_getinheritsched(), pthread_attr_getinheritsched(),
//             : pthread_attr_getstacksize()
//             : See pthread_once_t, pthread_t, pthread_attr_t, pthread_key_t
//             :
//============================================================================

#include <iostream>
#include <sstream>
#include <array>
#include <sched.h>
#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

static void*
doSomething (void* param);
static void
printThreadAttributes (pthread_attr_t attr);
static void
threadInit (void);
static void
threadDestroy (void *);
static void
printSum (void);

// Thread tracking object
#define NUM_THREADS  6
class TestInfo
{
public:
  pthread_t myThreadInfo;          // thread info for thread
  pthread_t mainThreadInfoValue;   // thread info for main thead.
  long int randomValue;            // a random number which will be summed.
  TestInfo () :
      myThreadInfo(0), mainThreadInfoValue(0),
	  randomValue(rand () / NUM_THREADS)
  {
    cout << "TestInfo:ctor(): randomValue=" << randomValue << endl;
  }
  std::string
  toString()
  {
    std::stringstream s = std::stringstream();
    s << "myThreadInfoPtr=" << myThreadInfo << " mainThreadInfoValue=" << mainThreadInfoValue << endl;
    std::string str = s.str ();
    return str;
  }
};

// Static class thread tracking static array object
typedef std::array<TestInfo, NUM_THREADS> MyTestInfo;
static MyTestInfo myTestInfo{ };  // initializes with default constructor

// vector for tracking pthread_key_t keys
static std::vector<pthread_key_t> *threadMap = 0;

static int randomSum = 0;                // sum of all pthread randomValues from TestInfo
static pthread_mutex_t* randomSumMutex;  // mutex to protect randomSum
static pthread_cond_t *cv;               // condition variable

//static pthread_cond_t fakeCond   = PTHREAD_COND_INITIALIZER;
//static pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;


// The static array myTestInfo gets statically initialized with 6 TestInfo objects prior to main()
// main() creates a bunch of threads. Each thread adds a random number to a global sum value and waits 5 seconds.
// The updating and the waiting are mutex protected. Then the thread exists.
// The main thread waits for each thread to complete, then when all have completed, prints out the global sum.

int
main()
{
  printf("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

  int retVal = 0;
  // print thread attributes
  pthread_attr_t attr;
  if ((retVal = pthread_attr_init(&attr)) != 0)
  {
      cout << "pthread_attr_init() failed; exiting:" << retVal << endl;
      pthread_exit (NULL);
  }
  printThreadAttributes(attr);

  // print out the main thread id
  pthread_t myThread = pthread_self();
  std::stringstream s1 = std::stringstream();
  s1 << "\n main threadId = " << myThread << endl;
  std::cout << s1.str();

  // initialize pthread_key_t vector object  via the method threadinit.
  pthread_once_t once = PTHREAD_ONCE_INIT;
  if(( retVal = pthread_once( &once, threadInit )) != 0 )
  {
      cout << "pthread_once() failed; exiting" << retVal << endl;
      pthread_exit(NULL);
  }

  // create a key for the main thread; the key can be used by each thread. Put this key in a vector of keys..
  pthread_key_t threadKey_1;
  if(( retVal = pthread_key_create( &threadKey_1, threadDestroy )) != 0)
  {
      cout << "pthread_key_create() failed; exiting:" << retVal << endl;
      pthread_exit (NULL);
  }
  threadMap->push_back( threadKey_1 );

  // get a string object for this main thread and associate it with the key.
  std::stringstream temp;
  temp << "main thread object key=threadKey_1:" << myThread << endl;
  if(( retVal = pthread_setspecific( threadKey_1, new std::string(temp.str())))!= 0)
  {
      cout << "thread_setspecific(threadKey_1) failed:" << retVal << endl;
      pthread_exit(NULL);
  }

  // create threads
  MyTestInfo::iterator iter;
  for(iter = myTestInfo.begin(); iter < myTestInfo.end(); iter++)
  {
	  // store parent thread id
	  iter->mainThreadInfoValue = myThread;

	  // create child thread with the attributes ( &attr) of the parent.
	  // thread starts running at doSomething.
	  // Pass the doSomething the iter as an argument.
      retVal = pthread_create( &iter->myThreadInfo, &attr, doSomething, (void*) iter );
      if (retVal == 0)
	  {
	     std::stringstream s;
	     s << "thread creation succeeded; threadId=" << iter->myThreadInfo << endl;
	     std::cout << s.str() << endl;
	  }
      else
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
	       sDone << "Joined with thread=" << iter->myThreadInfo << ":retVal= " << res->c_str ();
	       cout << sDone.str() << endl;;
	       delete( res );
	  }
      else cout << "bad result=" << retVal << endl;
  }

  // destroy main thread data structures.
  if( threadMap )
     delete threadMap;

  printSum();

  if( randomSumMutex )
    delete randomSumMutex;
  if( cv )
    delete cv;

  std::stringstream s;
  s << "END:  Function=" << __FUNCTION__ << " File=" << __FILE__ << " Line="
      << __LINE__;
  std::cout << s.str() << endl;

  pthread_exit( NULL );
  return 0;
}

static void*
doSomething( void* data )
{
  int retVal = 0;
  pthread_t id = ((TestInfo*) (data))->myThreadInfo;

  std::stringstream s;
  s << "Start:  Function=" << __FUNCTION__ << " File=" << __FILE__ << " Line="
      << __LINE__ << " threadID=" << id;
  std::cout << s.str() << endl;

  // return string info;  the main thread will destroy the memory when it joins.
  std::stringstream retStatus;
  retStatus << "Thread id" << id << " terminating" << endl;
  std::string *termStatus = new std::string (retStatus.str());

  if ( pthread_self() != id )
  {
      std::stringstream errormsg;
      errormsg << "thread ID mismatch; myThread=" << pthread_self() << " passed threadID=" << id << endl;
      pthread_exit( (void*)termStatus );
  }

  // get the key from the vector and create an associated thread specific string
  pthread_key_t key = threadMap->front();
  if( key == NULL )
  {
      cout << "key is NULL << endl";
      pthread_exit( (void*)termStatus );
  }

  // check if the thread key is associated with a value; it should not be.
  void* ptr;
  if(( ptr = pthread_getspecific(key)) != NULL )
  {
      // error; key with a valid value should not exist yet..
      cout << "error:key already has a value " << endl;
      pthread_exit( (void*)termStatus );
  }

  // associate the key with the thread specific string.
  std::stringstream temp;
  temp << "thread=" << id << ";threadkey=" << key << endl;
  if( (retVal = pthread_setspecific (key, new std::string (temp.str ()))) != 0 )
  {
      cout << "error:pthread_setspecific(key) failed " << retVal << endl;
      pthread_exit( (void*)termStatus );
  }

  // update the global sum; protect with mutex

  pthread_mutex_lock( randomSumMutex );
  int randomSumOld = 0;
  randomSumOld = randomSum;
  randomSum = randomSum + ( (TestInfo*)(data))->randomValue;

  // print data
  std::stringstream s5 = std::stringstream();
  s5 << "\nadding randomValue=" <<  ((TestInfo*)(data))->randomValue << " to randomSum=" << randomSumOld << " to get new randomSum=" << randomSum;
  std::cout << s5.str() << endl;
  sleep(5);
  pthread_mutex_unlock( randomSumMutex );

  /*
   // wait for 3 seconds
   struct timeval now;
   gettimeofday(&now,NULL);

   struct timespec to;
   to.tv_sec = now.tv_sec + 5;
   to.tv_nsec =  (now.tv_usec+1000UL*1000)*1000UL;


   if ((retVal = pthread_cond_timedwait( &fakeCond, &fakeMutex, &to)) == ETIMEDOUT)
   {
   cout << "timed out" << endl;
   }
   else
   {
   cout << "retval="<< retVal << endl;
   }

   pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;
   pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;

   //if ((retVal = pthread_cond_wait( &fakeCond, &fakeMutex)) == ETIMEDOUT)


   pthread_mutex_lock(&fakeMutex);
   retVal = pthread_cond_wait( &fakeCond, &fakeMutex);
   cout << "retval="<< retVal << endl;
   pthread_mutex_unlock(&fakeMutex);
   */
  std::stringstream s4;
  s4 << "END:  Function=" << __FUNCTION__ << " File=" << __FILE__ << " Line="
      << __LINE__ << " threadID=" << id;
  std::cout << s4.str() << endl;
  pthread_exit( (void*)termStatus );
  return( NULL );
}

// print out thread attributes available in Cygwin:
// thread attributes are contained in "attr".
static void
printThreadAttributes( pthread_attr_t attr )
{

   cout << "print attributes of threads as implemented in Cygwin" << endl;

  int attr_value_int = 0;
  // thread state
  if (!pthread_attr_getdetachstate( &attr, &attr_value_int ) )
  {
      cout << "detachState=" << attr_value_int
	  << (attr_value_int ? ":PTHREAD_CREATE_DETACHED" : ":PTHREAD_CREATE_JOINABLE") << endl;
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
  if( !pthread_attr_getschedpolicy( &attr, &attr_value_int ))
  {
      std::array<std::string, 3> policy
	              { ":PTHREAD_PRIO_NONE", ":PTHREAD_PRIO_INHERIT", ":PTHREAD_PRIO_PROTECT" };
      cout << "schedPolicy=" << attr_value_int << policy[attr_value_int] << endl;
  }

  // thread scope;
  if( !pthread_attr_getscope( &attr, &attr_value_int ))
  {
      std::array<std::string, 2> scope { ":PTHREAD_SCOPE_PROCESS", ":PTHREAD_SCOPE_SYSTEM" };
      cout << "thread scope=" << attr_value_int << scope[attr_value_int] << endl;
  }

  // stack size;
  unsigned long attr_value_long;
  if( !pthread_attr_getstacksize( &attr, &attr_value_long ))
  {
      cout << "stack size=" << attr_value_long << endl;
  }

}

// set up via pthread_once();
static void
threadInit( void )
{
  // initialize thread array if not done already.
  if ( !threadMap )
  {
	  threadMap = new std::vector<pthread_key_t>;
  }
  // create and initialize a mutex to be used to protect variable randomSum.
  // Upon successful initialization, the state of the mutex becomes initialized and unlocked since NULL is default
  // There are two types of attributes: type ( with a default of PTHREAD_MUTEX_NORMAL and
  // robust( with a default of PTHREAD_MUTEX_ROBUST)
  randomSumMutex = new pthread_mutex_t( PTHREAD_MUTEX_INITIALIZER );
  if( pthread_mutex_init( randomSumMutex, NULL ) != 0 )
   {
       cout << "pthread_mutex_init() failed" << endl;
   }

  // create a thread condition and initialize it.
  cv = new pthread_cond_t( PTHREAD_COND_INITIALIZER );
  if( pthread_cond_init( cv, NULL ) != 0 )
  {
      cout << "pthread_cond_init() failed" << endl;
  }


  cout << "threadInit() done" << endl;
  return;
}

// set up as part of pthread_create_key(); called on pthread_exit()
static void
threadDestroy( void* ptr )
{
  if( ptr != NULL )
  {
      // ptr should be a std::string pointer set up with pthread_create_key()
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

static void
printSum()
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
  if( sum != myRandomSum )
  {
      cout << "error: sum=" << sum << " :randomSum=" << myRandomSum << endl;
  }
  else
  {
      cout << "Success: sum=" << sum << " :randomSum=" << myRandomSum << endl;
  }
}
