/*
 * MyLogger.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: larry burley
 *      A simple logger class that logs to the console.  It's major feature is that each log entry is atomic and won't get
 *      interspersed with other log entries in a multi-threading application.  This is because cout is not thread safe
 *      So instead, use stringstream to form the string, then stream it to cout with one "<<" sequence. Multiple "<<" on
 *      cout can cause interspersion of output between threads concurrently using cout.
 *
 *      Note:
 *      #define BOOST_CHRONO_VERSION 2 issues on Cywin..
 *      There are two versions of the input/output chrono functions since Boost 1.52.0.
 *      which affects the compilation of time_fmt().
 *      Since Boost 1.55.0, the newer version is used by default.
 *      If you use a version older than 1.55.0, you must define the macro BOOST_CHRONO_VERSION
 *      and set it to 2
 *      chrono v1 includes;  does not work on cygwin...
 *      #include <boost/chrono/io_v1/chrono_io.hpp>
 *      v2 includes; works on cygwin., But need to set
 *      #define BOOST_CHRONO_VERSION 2 in file chrono/config.hpp.
 */

#include "MyLogger.hpp"
#include <sstream>
#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

void
MyLogger::log(const char *file, const char* function, int line, const std::string& str)
{
   std::stringstream ss = std::stringstream();
   ss << boost::chrono::system_clock::now() << " : "
	  << "  thread id:" << boost::this_thread::get_id()
	  << "       File:" << file
      << "   Function:" << function
      << "       Line:" << line
      << " Data: " << str << endl;
    cout << ss.str();
	return;
};

// logs the beginning of a method
void
MyLogger::logStart(const char *file, const char* function, int line )
{

   std::stringstream ss = std::stringstream();
   ss << boost::chrono::system_clock::now() << " : "
      << "  thread id:" << boost::this_thread::get_id()
      << " Start File:" << file
	  << "   Function:" << function
      << "       Line:" << line    << endl;
   cout << ss.str();
   return;
};

// logs the end  of a method
void
MyLogger::logEnd(const char *file, const char* function, int line )
{


   std::stringstream ss = std::stringstream();
   ss << boost::chrono::system_clock::now() << " : "
      << "  thread id:" << boost::this_thread::get_id()
	  << "   End File:" << file
      << "   Function:" << function
	  << "       Line:" << line    << endl;
   cout << ss.str();
   return;
};


