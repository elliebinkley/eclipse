


//#define BOOST_CHRONO_VERSION 2

// There are two versions of the input/output chrono functions since Boost 1.52.0.
// which affects the compilation of time_fmt().
// Since Boost 1.55.0, the newer version is used by default.
// If you use a version older than 1.55.0, you must define the macro BOOST_CHRONO_VERSION
// and set it to 2
// chrono v1 includes;  does not work on cygwin...
// #include <boost/chrono/io_v1/chrono_io.hpp>
// v2 includes; works on cygwin., But need to set
// #define BOOST_CHRONO_VERSION 2 in file chrono/config.hpp.

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

MyThreadBoost::MyThreadBoost( const std::string& description ) : m_description(description)
{
  T_START;
  T_LOG("thread created:" + description);
  T_END;
}

// callable operator
void
MyThreadBoost::operator()()
{
    T_START;
    stringstream s1;
    s1 << " thread id:" << m_thread->get_id();
    m_description.append(s1.str());
   	try
   	{
   	  for( int i = 0; i < 3; i++ )
   	  {
   		  T_START;
   		  // get the mutex
   		  MyThreadBoost::m_mtx.lock();
   		  stringstream ss;
   		  ss << m_description << " :sleeping for " << m_numSeconds << " for " << i << "th time with the mutex ";
   		  std::string s = ss.str();
 		  T_LOG( s);

   		  boost::this_thread::sleep_for( boost::chrono::seconds( m_numSeconds ) );
   		  // unlock mutex;
   		  MyThreadBoost::m_mtx.unlock();
   		  T_END;
   	  }
   	}
   	catch (boost::thread_interrupted&)
   	{
   		 T_START;
   		 MyThreadBoost::m_mtx.unlock();
   		 cout << "thread id=" << boost::this_thread::get_id() << " interrupted" << endl;
   		 T_END;
   	}
    T_END;
}

MyThreadBoost::~MyThreadBoost()
{
  T_START;
  if( m_thread != nullptr)
  {
	  m_thread->interrupt();
  }
  T_END;
}

void MyThreadBoost::join()
{
	if (m_thread != nullptr)
	{
		if (m_thread->joinable())
		{
			std::stringstream s;
			s << "waiting to join thread=" <<  m_thread->get_id();
			T_LOG(s.str());
			m_thread->join();
		}
		else
		{
			cout << "thread=" << m_thread->get_id() << "not joinable" << endl;
		}
	}
}

void
MyThreadBoost::run()
{
    T_START;
    this->m_thread = new boost::thread(boost::ref(*this)); // invokes callable operator
    T_END;
}


