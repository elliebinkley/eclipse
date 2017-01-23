//============================================================================
// Name        : BootThreads.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <boost/thread.hpp>
#include "MyThreadBoost.hpp"

using namespace std;
using namespace boost;



int main()
{
    T_START;
    MyThreadBoost *threadExample = new MyThreadBoost("Example of Boost usage");
    MyThreadBoost test(*threadExample);
    threadExample->doSomething();
    if( threadExample ) delete threadExample;
	T_END;
	return 0;
}
