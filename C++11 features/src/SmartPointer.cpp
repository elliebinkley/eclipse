/*
 * SmartPointer.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: USER
 *      C++11 includes new smart pointer classes: shared_ptr and the recently-added unique_ptr.
 *      Both are compatible with other Standard Library components, so you can safely store these smart pointers
 *      in standard containers and manipulate them with standard algorithms.
 */

#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <cstdlib>
#include <vector>
#include "SmartPointer.hpp"
#include "Utilities.hpp"

using namespace std;

namespace SmartPtr
{
// singleton initialization
SmartPointer* SmartPointer::m_smartPointer = nullptr;

SmartPointer*  SmartPointer::getInstance(void)
{
	if( SmartPointer::m_smartPointer == nullptr )
	{
		// lazy instantiation; singleton
		SmartPointer::m_smartPointer = new SmartPointer();
	}
	cout << "got it" << endl;
	return m_smartPointer;
}

void SmartPointer::runTests()
{
	try
	{
	    smartPointerTest_1();
        smartPointerTest_2();
        smartPointerTest_3();
        smartPointerTest_4();
        smartPointerTest_5();
		return;
	} catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}

// most basic usage of smart pointer.
void
SmartPointer::smartPointerTest_1()
{
	T_START;
	std::unique_ptr<A> p1(new A("a10"));
	p1->print();
	T_END;
}

// Test vectors of classes that contain smart pointers.
void
SmartPointer::smartPointerTest_2()
{
	T_START;
    std::vector<std::unique_ptr<A>> vectorSP;
    for( int i = 0; i < 10; i ++)
    {
    	std::stringstream s4;
    	s4 << i << " ";
    	vectorSP.push_back(std::unique_ptr<A> (new A(s4.str())));
    }

    for( const auto &n : vectorSP )
    {
        n->print();
    }

	T_END;
}

// test copy constructor when class has a smart pointer.
void
SmartPointer::smartPointerTest_3()
{
	T_START;

	// make an A with default constructor
	A a("test copy constructor");
	a.print();

	// copy it
	std::unique_ptr<A> p1(new A(a));
	// print both
	a.print();
	p1->print();

	T_END;
}

// test default constructor with smart  pointer;
// check that class member which is a smart pointer is  initialized correctly.
void
SmartPointer::smartPointerTest_4()
{
	T_START;
	// default constructor of A using smart pointer
    std:unique_ptr<A> p1(new A());

    // see if default values of A smart [pointer members are printed correctly.
    p1->print();
    T_END;
}


// test move constructor with smart  pointer in the class being moved.
void
SmartPointer::smartPointerTest_5()
{
	T_START;
	// default constructor of A.
    A a("instance of class A");
    a.print();
    A a1 = std::move(a);
    a.print();
    a1.print();
    T_END;
}

}   // end of namespace SmartPtr

