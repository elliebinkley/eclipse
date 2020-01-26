<<<<<<< HEAD
//#define BOOST_CHRONO_VERSION 2 issues on Cywin..

// There are two versions of the input/output chrono functions since Boost 1.52.0.
// which affects the compilation of time_fmt().
// Since Boost 1.55.0, the newer version is used by default.
// If you use a version older than 1.55.0, you must define the macro BOOST_CHRONO_VERSION
// and set it to 2
// chrono v1 includes;  does not work on cygwin...
// #include <boost/chrono/io_v1/chrono_io.hpp>
// v2 includes; works on cygwin., But need to set
// #define BOOST_CHRONO_VERSION 2 in file chrono/config.hpp.
=======
/*
 * See Readme for description.
 */
>>>>>>> 42789e912da60a7aaf737f539995506daf06f10a

#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono/config.hpp>
#include <boost/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>
#include <boost/locale.hpp>
#include <sstream>
#include "MyThreadBoost.hpp"

using namespace boost;
using namespace std;

#define NUM_SECONDS 3

boost::mutex MyThreadBoost::m_mtx;
boost::condition_variable MyThreadBoost::m_cv;

bool MyThreadBoost::m_isEmpty = true;
uint32_t MyThreadBoost::m_numWaiting = 0;
boost::thread::id MyThreadBoost::m_gotIt;

MyThreadBoost::MyThreadBoost( const std::string &description ) : m_description( description )
{
   T_LOG( "thread created:" + description );
}

// callable operator
// each threads independently attempts to get ownership; own for three seconds, else wait;
// do this 3 times then stop.
void MyThreadBoost::operator()()
{
   stringstream s1;
   s1 << " thread id:" << m_thread->get_id();
   m_description.append( s1.str() );

   try
   {
      for( int i = 0; i < 3; )
      {
         {
            // lock mutex
            boost::unique_lock<boost::mutex> lck{m_mtx};
            // give other threads a chance to run....
            // if no thread is running and no other threads are waiting or if they are waiting, then if this thread isn't the
            // last one to run, then this tread can run.
            if (( this->m_isEmpty ) &&
                ( m_numWaiting == 0 || m_gotIt != this->m_thread->get_id() )
               )
            {
               MyThreadBoost::m_gotIt = this->m_thread->get_id();
               this->m_isEmpty = false;

//             stringstream ss3;
//             ss3 << m_description << " :got it!!; i=" << i;
//             T_LOG( ss3.str() );
            }
            else
            {
               // another thread has it; wait;
               // wait causes unlocking of mutex
               stringstream ss3;
               ss3<< m_description << " :waiting!!!; i=" << i;
//               T_LOG( ss3.str() );
               m_numWaiting++;
               MyThreadBoost::m_cv.wait(lck);
               // awoken; mutex is automatically relocked on wakeup
               // should be empty.

//             stringstream ss4;
//             ss4 << m_description << " :awaken!!!; i=" << i;
//             T_LOG( ss4.str() );

               m_numWaiting--;
               continue;
            }
         }   // mutex unlocked since lock goes out of scope

         boost::this_thread::sleep_for( boost::chrono::seconds( m_numSeconds ) );

         stringstream ss1;
         ss1 << m_description << " :waking from sleep; i=" << i;
         T_LOG( ss1.str() );


         {
            boost::unique_lock<boost::mutex> lck{m_mtx};
            // should not be empty and this thread should have it
            assert( !m_isEmpty );
            assert( m_gotIt ==  m_thread->get_id() );

//          stringstream ss3;
//          ss3 << " m_isEmpty=" <<  m_isEmpty;
//          ss3 << " m_gotIt=" <<  m_gotIt  << " getid()=" << m_thread->get_id();
//          T_LOG( ss3.str() );

            this->m_isEmpty = true;
            m_cv.notify_one();
         }  // unlock mutext via lck out of scope

         i++;
      }
   }
   catch( boost::thread_interrupted& )
   {
      stringstream ss2;
      MyThreadBoost::m_mtx.unlock();
      ss2 << "thread id=" << boost::this_thread::get_id() << " interrupted";
      std::string s = ss2.str();
      T_LOG( s );
   }
}

MyThreadBoost::~MyThreadBoost()
{
   stringstream ss;
   ss << "thread id=" << boost::this_thread::get_id() << " DTOR()";
   std::string s = ss.str();
   T_LOG( s );

   if( m_thread != nullptr )
   {
      m_thread->interrupt();
   }
}

void MyThreadBoost::join()
{
   if( m_thread != nullptr )
   {
      if( m_thread->joinable() )
      {
//       std::stringstream s;
//       s << "waiting to join thread=" << m_thread->get_id();
//       T_LOG( s.str() );
         m_thread->join();
      }
      else
      {
//       std::stringstream s;
//       s << "thread=" << m_thread->get_id() << "not joinable" << endl;
//       T_LOG( s.str() );
      }
   }
}

void MyThreadBoost::run()
{
   m_thread = new boost::thread( boost::ref( *this ) ); // invokes callable operator
}

