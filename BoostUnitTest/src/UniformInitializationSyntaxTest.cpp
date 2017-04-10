/*
 * UniformInitializationSyntax.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: Burley
 *      Brief: Demo various new types of C++11 Initialization Syntax, in particular using brackets.
 *      Description:C++11 allows new types of Initialization Syntax. These techniques make initialization of classes,
 *      class members similar to existing initialization of primitive types.
 *
 *      This class exercises those techniques.
 *      1.  array initialization with new and using bracket initialization
 *          See arrayInit()
 *          int* a = new int[3] { 1, 2, 0 }; /C++11 only
 *      2. C++11, member array initializer list via brackets.
 *         See memberArrayInit_0() and memberArrayInit_1()
 *         class X {
 *                 int a[4];
 *                 public:
 *                  X() : a{1,2,3,4} {}  /C++11 only
 *         };
 *      3. C++11; container initialization via bracket initialization
 *         see containerInitialization()
 *         vector<string> vs={ "first", "second", "third"};  //C++11 only
 *      4. C++11; In class Initialization
 *         See inClassInitialization()
 *         Similarly, C++11 supports in-class initialization of data members:
 *         class C
 *         {
 *             int a=7; //C++11 only
 *             public:
 *             C();
 *         };
 *
 *       Note:See discussion on PODs and aggregates at
 *       http://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special
 */

#include <BoostUnitTest/src/UniformInitializationSyntaxTest.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;
using namespace UniformInit;

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( UniformInitialization )
BOOST_AUTO_TEST_CASE( ArrayInitialization )
{

    // Create multiple objects inside  once with new and initialize them inside an array
    // new for C++11.
    A* a = new A[3]
    { A ( 3, std::string ("A0") ),
      A ( 4, std::string ("A1") ),
      A ( 5, std::string ("A2") )
    };

    BOOST_CHECK( a[0].getDesc() == std::string ("A0") );
    BOOST_CHECK( a[1].getDesc() == std::string ("A1") );
    BOOST_CHECK( a[2].getDesc() == std::string ("A2") );
    BOOST_CHECK( a[0].getVal()  == 3 );
    BOOST_CHECK( a[1].getVal()  == 4 );
    BOOST_CHECK( a[2].getVal()  == 5 );

}
BOOST_AUTO_TEST_CASE( ObjectInitialization )
{

    // 3 different styles of initialization; all equivalent; this exists for string in C++ 03.
    string s0( "whatever" );
    string s1{ "whatever" };
    string s2 = "whatever";
    BOOST_CHECK( s0 == s1);

    // similarly, 3 different types of initialization for objects; all equivalent
    // C++11 only
    X x0 ( s0 );
    X x1 { s0 };
    X x2 = { s0 };
    BOOST_CHECK( x0 == x1);
    BOOST_CHECK( x0 == x2);
}

BOOST_AUTO_TEST_CASE( MemberInitializationWithBrackets )
{
    // declare a class with a member that is an array of std::string
    class A_Temp
    {
    public:
       // C++11 only; initialize members with brackets inside a class
       A_Temp() : m_desc { "A0", "A1", "A2", "A3" }  {};

       void toString ()
       {
           for (int i = 0; i < 4; i++)
           {
               std::cout << m_desc[i] << std::endl;
           }
       };

       std::string m_desc[4];
    };

    // prove that the member initialization with brackets worked ..
    A_Temp a;
    BOOST_CHECK( a.m_desc[0] == std::string("A0") );
    BOOST_CHECK( a.m_desc[1] == std::string("A1") );
    BOOST_CHECK( a.m_desc[2] == std::string("A2") );
    BOOST_CHECK( a.m_desc[3] == std::string("A3") );
}

BOOST_AUTO_TEST_CASE( ContainerMemberInitialization)
{
    // in C++11 containers can be initialized with brackets. Write a test case to demonstrate
    class A_TEMP
    {
    public:
        typedef std::array<int,6> ARRAY_INT;
        typedef std::array<char,5> ARRAY_CHAR;
        A_TEMP() : m_arrayInt { 0, 1, 2, 3, 4, 5 } {};
        A_TEMP ( const A_TEMP& a ) : m_arrayInt( a.m_arrayInt ), m_arrayChar( a.m_arrayChar ) {};
        A_TEMP& operator= ( const A_TEMP& a )
        {
            if( this == &a ) return *this;
            this->m_arrayInt = a.m_arrayInt;
            this->m_arrayChar = a.m_arrayChar;
            return *this;
        }

        std::string
        toString() const
        {
            std::stringstream s;
            for( auto const &n : m_arrayInt )
            {
                s << " " << n;
            }
            s << "\n";
            for( auto &n : m_arrayChar )
            {
                s << " " << n;
            }
            return s.str();
        }

        const ARRAY_INT&
        getIntegers() const
        {
            return m_arrayInt;
        }

        const ARRAY_CHAR&
        getChars() const
        {
            return m_arrayChar;
        }

    private:
        std::array<int, 6> m_arrayInt;
        ARRAY_INT  m_arrayMyInt;
        ARRAY_CHAR  m_arrayMyChar = {{ 'a', 'b', 'c', 'd', 'e' }};

        // I have no idea why double  brackets are required....
        // C++11 only; initialize containers via brackets
        std::array<char, 5> m_arrayChar = {{ 'a', 'b', 'c', 'd', 'e' }};
    };

    std::vector<A_TEMP> vector_a { A_TEMP(), A_TEMP() };  // C++11
    for (auto const &v : vector_a)              // C++11
    {
        A_TEMP::ARRAY_INT i_array = v.getIntegers();
        int i = 0;
        for (auto const &myInt : i_array)
        {
            BOOST_CHECK( myInt == i );
            i++;
        }

        A_TEMP::ARRAY_CHAR c_array = v.getChars();
        char j = 'a';
        for( auto &myChar : c_array )
        {
            BOOST_CHECK( myChar == j );
            j++;
        }
    }
}
BOOST_AUTO_TEST_SUITE_END()



