/*
 *  myThread.h
 *
 *  Created on: Dec 8, 2016
 *      Author: USER
 *      Create N Boost:thread from main and have the main thread wait for the N created threads.
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
using namespace boost;

class ThreadBoostTester
{
public:
	static ThreadBoostTester* instance();
	void runThreadBoostTests();

private:
	// make private the constructor;use the default implementation
	ThreadBoostTester() = default;
	// make private the copy constructor;use the default implementation
	ThreadBoostTester(const ThreadBoostTester& o)= default;
	// make private the =  operator;use the default implementation
	ThreadBoostTester& operator= (const ThreadBoostTester& o) = default;
	// default does not apply to == operator; so write an implementation; make it private;
	// as a singleton, this should never get called
	friend bool operator== (const ThreadBoostTester& rhs, const ThreadBoostTester lhs)
    {
	    return (rhs == lhs);
	};

	static ThreadBoostTester* m_ThreadBoostTesterPtr;
};

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
