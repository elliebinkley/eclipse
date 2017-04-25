/*
 * ConstructionTest.cpp
 *
 *  Created on: Feb 12, 2017
 *      Author: Burley
 *      Test various types of construction methods using C++ syntax.
 *      use keywords "default" and "deleted" on constructors.
 *      use bracket initialization.
 *      use move constructor
 *      use std::move()
 *
 *
 */

#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE C++11+ Syntax Checker    // Master suite with the main() testRunner.
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( Construction )

// declare/define a class A and provide various standard construction methods using C++11 syntax.
// Then test them with Boost...
class A
{
public:
    // 1. default constructor
    A()  {}

    // 2. user supplied constructor
    A( const std::vector<int>& input ) : m_vec( input )  {}

    // 3. copy constructor
    A( const A& a ) : m_vec( a.m_vec ) {}

    // 4. copy assignment operator
    A& operator=( const A& a )
    {
        if( this == & a )
            return (* this);
        this->m_vec = a.m_vec;
        return (* this);
    }

    // 5. move constructor; need to use std::move on all members.
    //    C++ 11
    //    Note: all STL containers support std::move.
    A( A&& o ) noexcept : m_vec(std::move(o.m_vec))  {}

    // 6. move assignment operator
    //    C++ 11
    //    Note: all STL containers support std::move.
    A& operator=(A&& other)
    {
        m_vec = std::move(other.m_vec);
        return *this;
    }
    // 7. destructor
    ~A() { }

    std::string toString()
    {
        std::stringstream ss;
        for( auto const &n : m_vec)
        {
            ss << n << " ";
        }
        return ss.str();
    }
    bool isValid()
    {
        return m_isValid;
    }

    const std::vector<int>& getVector() const
    {
        return m_vec;
    }

    // 8. == operator
    /* cannot declare friend class in a local class
     friend bool operator==(const A& rhs, const A& lhs) const
     {
     return ( rhs.m_vec == lhs.m_vec);
     }
     */
private:
    std::vector<int> m_vec { 10,11,12,13 };  // C++11 initializer list
    bool m_isValid = true;// used for testing successful construction
};


BOOST_AUTO_TEST_CASE( UserSuppliedDefaultConstruction )
{
    A a;
    BOOST_CHECK( a.isValid() );
    BOOST_CHECK( a.toString() == "10 11 12 13 " );
}


BOOST_AUTO_TEST_CASE( UserSuppliedConstruction )
{
    std::vector<int> j { 20, 21, 22, 23, 24, 25 };
    A a( j );
    BOOST_CHECK( a.getVector() == j );
}

BOOST_AUTO_TEST_CASE( UserSuppliedCopyConstruction )
{
    A a1(std::vector<int> { 20, 21, 22, 23, 24, 25 });
    A a2( a1 );
    BOOST_CHECK( a1.isValid() );
    BOOST_CHECK( a2.isValid() );
    BOOST_CHECK( a2.getVector() == a1.getVector() );
}

BOOST_AUTO_TEST_CASE( UserSuppliedAssignmentConstruction )
{
    A a1(std::vector<int> { 20, 21, 22, 23, 24, 25 });
    A a2 = a1;
    BOOST_CHECK( a1.isValid() );
    BOOST_CHECK( a2.isValid() );
    BOOST_CHECK( a2.getVector() == a1.getVector() );
}

BOOST_AUTO_TEST_CASE( UserSuppliedMoveConstruction )
{
    std::vector<int> before { 20, 21, 22, 23, 24, 25 };
    std::vector<int> after;
    A a1(before);
    A a2 = std::move( a1 );
    BOOST_CHECK( a1.isValid() );
    BOOST_CHECK( a2.isValid() );
    BOOST_CHECK( a1.getVector() == after );
    BOOST_CHECK( a2.getVector() == before );
}


BOOST_AUTO_TEST_SUITE_END()
