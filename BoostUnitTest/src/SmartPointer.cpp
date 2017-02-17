/*
 * SmartPointer.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Larry Burley
 *      C++11 includes new smart pointer classes: shared_ptr and unique_ptr.
 *      Both are compatible with other Standard Library components, so you can safely store these smart pointers
 *      in standard containers and manipulate them with standard algorithms.
 *      This program uses various aspects of the std:unique_ptr.
 */

#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <cstdlib>
#include <vector>
#include "SmartPointer.hpp"
#include <SharedLibrary/inc/MyLogger.hpp>



#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace SmartPtr;


BOOST_AUTO_TEST_SUITE( std_SmartPointer )
BOOST_AUTO_TEST_CASE( basic_unique_ptr)
{
    // most basic usage of smart pointer.
    std::unique_ptr<A> p1(new A ("a10"));
    BOOST_CHECK( p1 );
    BOOST_CHECK( p1->isValid() );
    std::string s = p1->toString();
}

BOOST_AUTO_TEST_CASE( vector_unique_ptr)
{
    // create a smart pointer to a vector and the add smart pointer strings to it.
    std::vector<std::unique_ptr<A>> vectorSP;
    for(int i = 0; i < 10; i++ )
    {
        std::stringstream ss;
        ss << i << " ";
        vectorSP.push_back(std::unique_ptr<A> (new A (ss.str ())));
    }

    for( const auto &n : vectorSP )
    {
        BOOST_CHECK( n );             // bool operator on smartpointer for A
        BOOST_CHECK( n->isValid() );  // check if A is valid.
        n->toString();
    }
}

BOOST_AUTO_TEST_CASE( copyConstructorSmartPointer)
{
    // test copy constructor when class A has a smart pointer.  Copy constructor with a smart pointer is normally
    // not allowed, since smart pointers cannot be copied, unless you write a specific implementation, which class A has.
    const char* ptr = "test copy constructor";
    A a( ptr );
    BOOST_CHECK( *( a.m_ptr_string ) == std::string(ptr));

    // copy ctor for A
    std::unique_ptr<A> p1 (new A (a));  // utilizes custom copy constructor that works with smart pointer

    // compare both
    BOOST_CHECK( *( p1->m_ptr_string ) == std::string(ptr));
    BOOST_CHECK( *( a.m_ptr_string ) == std::string(ptr));
}

BOOST_AUTO_TEST_CASE( defaultConstructorSmartPointer)
{
    // test default constructor with smart  pointer;
    // check that class member, which is a smart pointer, is initialized correctly.
    std::unique_ptr<A> p1( new A() );

    // see if default values of A smart pointer members are printed correctly.
    BOOST_CHECK( p1 );
    BOOST_CHECK ( (p1->m_ptr_string));
}

BOOST_AUTO_TEST_CASE( moveConstructorSmartPointer)
{
    // test move constructor with smart  pointer in the class being moved
    std::string s = "instance of class A";
    A a( s );
    A a1 = std::move( a );
    // Instance a1 should contain s
    BOOST_CHECK( *(a1.m_ptr_string) == s);
    // Instance a should not have a valid smart pointer anymore
    BOOST_CHECK( !(a.m_ptr_string) );
}

BOOST_AUTO_TEST_SUITE_END()







