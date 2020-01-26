/*
 *  See Readme for description
 *
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


class MyThreadBoost
{
public:
   MyThreadBoost( const std::string &description );

   // disallow copies since thread is inside this object
   MyThreadBoost( const MyThreadBoost& ) = delete;
   MyThreadBoost& operator=( const MyThreadBoost &arg ) = delete;

   // disallow moves since thread is inside this object. Although actually, the boost::thread is a movable object...
   MyThreadBoost( MyThreadBoost &&o ) noexcept = delete;
   MyThreadBoost& operator=( MyThreadBoost &&other ) = delete;

   ~MyThreadBoost();
   void operator()();
   void run();
   void join();
   static boost::thread::id m_gotIt;

private:
   std::string m_description;
   boost::thread *m_thread = nullptr;

   static const int m_numSeconds = 3;
   static boost::mutex m_mtx;
   static boost::condition_variable m_cv;
   static uint32_t m_numWaiting;

   static bool m_isEmpty;
};

#endif /* MYTHREADBOOST_HPP_ */
