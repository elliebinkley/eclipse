 /*
 * MyLogger.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: larry burley
 *      A simple logger class that logs to the console.
 *      Each log entry is atomic and won't get interspersed with other log entries in
 *      a multi-threading application.
 *      Since cout is not thread safe, multiple "<<" on cout can cause interspersion
 *      of output between threads.  So use std::stringstream
 *      to form the string, then stream it to cout with a single "<<" sequence.
 */

#include "MyLogger.hpp"
#include <sstream>
#include <iostream>
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/io/time_point_io.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

void MyLogger::log( const char *file, const char *function, int line, const std::string &str )
{
   std::stringstream ss = std::stringstream();
   std::cout << boost::chrono::time_fmt( boost::chrono::timezone::local, "%H:%M:%S" )
         << boost::chrono::system_clock::now() << '\n';
   ss << boost::chrono::time_fmt( boost::chrono::timezone::local, "%H:%M:%S" )
         << boost::chrono::system_clock::now() << " : " << "  thread id:"
         << boost::this_thread::get_id() << "       File:" << file << "   Function:" << function
         << "       Line:" << line << " Data: " << str << endl;
   cout << ss.str();
   return;
}

// logs the beginning of a method
void MyLogger::logStart( const char *file, const char *function, int line )
{
   std::stringstream ss = std::stringstream();
   ss << boost::chrono::time_fmt( boost::chrono::timezone::local, "%H:%M:%S" )
         << boost::chrono::system_clock::now() << " : " << "  thread id:"
         << boost::this_thread::get_id() << " Start File:" << file << "   Function:" << function
         << "       Line:" << line << endl;
   cout << ss.str();
   return;
}

// logs the end  of a method
void MyLogger::logEnd( const char *file, const char *function, int line )
{
   std::stringstream ss = std::stringstream();
   ss << boost::chrono::time_fmt( boost::chrono::timezone::local, "%H:%M:%S" )
         << boost::chrono::system_clock::now() << " : " << "  thread id:"
         << boost::this_thread::get_id() << "   End File:" << file << "   Function:" << function
         << "       Line:" << line << endl;
   cout << ss.str();
   return;
}

