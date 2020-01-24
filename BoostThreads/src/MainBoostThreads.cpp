//============================================================================
// Name        : MainBoostThreads.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/version.hpp>
#include <boost/container/vector.hpp>
#include "MyThreadBoost.hpp"

using namespace std;
using namespace boost;

int
main ()
{
  T_START;

  std::cout << "Using Boost: "
      << "\n boost version=" << BOOST_VERSION
      << "\n major version=" << BOOST_VERSION / 100000     << "."  // major version
      << "\n minor version=" << BOOST_VERSION / 100 % 1000 << "."  // minor version
      << "\n patch level=" << BOOST_VERSION % 100                  // patch level
      << "\n boost library version=" << BOOST_LIB_VERSION
      << std::endl;

  //  version of the chrono library
  std::cout << "\n Chrono Version=" <<  BOOST_CHRONO_VERSION << "\n" << endl;

  // info on the running machine ...
  cout << " number of physical threads on this system = " << boost::thread::physical_concurrency() << endl;
  cout << " number of hardware threads on this system = " << boost::thread::hardware_concurrency() << endl;

  unsigned int numMyThreads = boost::thread::hardware_concurrency();
  // make a boost vector that contains smart pointers to MyThreadBoost objects.
  T_LOG("creating threads.... ");
  boost::container::vector<std::unique_ptr<MyThreadBoost>> threadMap;
  for ( unsigned int i=0; i<numMyThreads; i++)
    {
      stringstream s;
      s << "ThreadBoost:" <<  i;
      std::unique_ptr<MyThreadBoost> p2(new MyThreadBoost( s.str() ));
      threadMap.push_back(std::move(p2));  // smartpointer  cannot be copied; they must be moved; p2 is now invalid..
    }

  T_LOG("running threads.... ");
  boost::container::vector<std::unique_ptr<MyThreadBoost>>::iterator iter;
  for( iter = threadMap.begin(); iter != threadMap.end(); iter++)
    {
      (*iter)->run();
    }

  T_LOG("main thread waits to join the child threads");
  boost::container::vector<std::unique_ptr<MyThreadBoost>>::iterator iter1;
  for( iter1 = threadMap.begin(); iter1 != threadMap.end(); iter1++)
    {
      (*iter1)->join();
    }

  T_END;
  return 0;
}
