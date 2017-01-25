/*
 * myThr
 ead.h
 *
 *  Created on: Dec 8, 2016
 *      Author: USER
 */

#ifndef MYTHREADBOOST_HPP_
#define MYTHREADBOOST_HPP_

#include <cstddef>
#include <string>

using namespace std;

#define T_START cout <<  "Start**  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << endl;
#define T_END cout <<    "End  **  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << endl;

class MyThreadBoost
{
public:
  MyThreadBoost(const std::string& description);
  MyThreadBoost(const MyThreadBoost&);
  ~MyThreadBoost ();
  MyThreadBoost&
  operator= (const MyThreadBoost& arg);
  void doSomething();
  static void threadDoSomething ();
private:
  std::string m_description;
  MyThreadBoost () : m_description (NULL)
  {};

};

#endif /* MYTHREADBOOST_HPP_ */
