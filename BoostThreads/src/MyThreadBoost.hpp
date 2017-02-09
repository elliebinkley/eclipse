/*
 * myThr
 ead.h
 *
 *  Created on: Dec 8, 2016
 *      Author: USER
 *      Create N Boost:thread from main and have the main thread wait for theN created threads.
 *      The created thread will timeout in 60 seconds.
 */

#ifndef MYTHREADBOOST_HPP_
#define MYTHREADBOOST_HPP_

#include <cstddef>
#include <string>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/mutex.hpp>
#include "MyLogger.hpp"

using namespace std;

/*

#define T_START std::stringstream s1 = std::stringstream(); \
	           s1 << boost::chrono::time_fmt(boost::chrono::timezone::local) \
                << boost::chrono::system_clock::now() << " : "  \
				<< "thread id:" << boost::this_thread::get_id()  \
                << " Start File:" << __FILE__ \
				<< " Function:" << __FUNCTION__ \
                << " Line:" << __LINE__    << endl; \
                cout << s1.str();


#define T_START1 cout \
	            << boost::chrono::time_fmt(boost::chrono::timezone::local) \
                << boost::chrono::system_clock::now() << " : "  \
				 << "thread id:" << boost::this_thread::get_id()  \
                << " Start File:" << __FILE__ \
				<< " Function:" << __FUNCTION__ \
                << " Line:" << __LINE__    << endl;

#define T_END cout \
	            << boost::chrono::time_fmt(boost::chrono::timezone::local) \
				<< boost::chrono::system_clock::now() << " : "   \
                << "thread id:" << boost::this_thread::get_id()  \
				<< " End File:" << __FILE__ \
				<< " Function:" << __FUNCTION__ \
                << " Line:" << __LINE__  << endl;

*/

class MyThreadBoost
{
public:
  MyThreadBoost( const std::string& description );

  // disallow copies since thread is inside this object
  MyThreadBoost( const MyThreadBoost& )= delete;
  MyThreadBoost&operator= ( const MyThreadBoost& arg ) = delete;

  // disallow moves since thread is inside this object. Although actually, the boost::thread is a movable object...
  MyThreadBoost( MyThreadBoost&& o ) noexcept  = delete;
  MyThreadBoost& operator=( MyThreadBoost&& other ) = delete;

  ~MyThreadBoost();
  void operator()();
  void run();
  static boost::mutex m_mtx;
  void join();


private:
  std::string m_description;
  boost::thread* m_thread = nullptr;
  static const int m_numSeconds = 3;
};


#endif /* MYTHREADBOOST_HPP_ */
