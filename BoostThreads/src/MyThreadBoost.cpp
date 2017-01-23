#include <algorithm>
#include <iostream>
#include <ostream>
#include <utility>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "MyThreadBoost.hpp"

using namespace std;

MyThreadBoost::MyThreadBoost(const std::string& description)
{
	T_START;
	m_description = description;
	T_END;
}

MyThreadBoost::MyThreadBoost(const MyThreadBoost& arg)
{

	m_description = arg.m_description;
	T_END;
}

MyThreadBoost& MyThreadBoost::operator=(const MyThreadBoost& arg)
{
	T_START;
	m_description = arg.m_description;
	T_END;
    return *this;
}

MyThreadBoost::~MyThreadBoost()
{
	T_START;
	T_END;
}
void doIt()
{
	T_START;
	T_END;
}
void MyThreadBoost::doSomething()
{
	T_START;
	boost::thread t{MyThreadBoost::threadDoSomething};
	t.join();
	T_END;
}

void MyThreadBoost::threadDoSomething()
{
	for( int i = 0; i < 20; i++ )
	{
		int seconds = 3;
		boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
		cout << "thread waiting" << endl;
	}
}

