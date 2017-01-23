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

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include "UniformInitializationSyntax.hpp"
#include "Utilities.hpp"

using namespace std;

namespace UniformInit
{

// Define the static pointer for the singleton in the cpp class and set it to zero;
// Note: do not use static keyword on definition; just declaration
// nullptr valid as of C++11
UniformInitializationSyntax* UniformInitializationSyntax::uniformInitializationSyntaxPtr = nullptr;

// get the singleton instance
UniformInitializationSyntax*
UniformInitializationSyntax::getInstance()
{
	 if (uniformInitializationSyntaxPtr == nullptr) // nullptr valid as of C++11
	 {
		 // lazy instatiation... Only do once..
		 uniformInitializationSyntaxPtr = new UniformInitializationSyntax();
	 }
	 return uniformInitializationSyntaxPtr;
}

// run all tests in this suite
void
UniformInitializationSyntax::
runTests()
{
	arrayInit();
	memberArrayInit_0();
	memberArrayInit_1();
	containerIntialization();
	inClassInitialization();
    return;
}

// see comments above for arrayInit()
void
UniformInitializationSyntax::
arrayInit()
{
	 T_START;
	 // declare a class with integer and string members.
	 class A
	 {
	 public:
		 A( const int value, const std::string& description):m_a(value),m_desc(description) {};
		 std::string toString()
		 {
			 std::stringstream s;
			 s << m_desc.c_str() << ":m_a=" << m_a << std::endl;
			 return s.str();
		 };
	 private:
		 int m_a;
		 std::string m_desc;
	 };

	 // Create multiple objects at once with new and initialize them.
	 // C++11 only.
	 A* a = new A[3] { A(3, std::string("A0")),
		               A(4, std::string("A1")),
					   A(5, std::string("A2"))};
	 for( int i = 0; i < 3; i++)
	 {
		cout << a[i].toString().c_str() << endl;
	 }

	 // 3 different types of initialization for string; all equivilent
	 string s0 ("whatever");
	 string s1 {"whatever"};
	 string s2 = "whatever";
	 if( s0 != s1) cout << "bug: s0!= s1" << endl;
	 if( s0 != s2) cout << "bug: s0!= s1" << endl;

	 // similarly, 3 different types of initialization for objects; all equivilent
	 // C++11 only
	 X x0(s0);
	 X x1 { s0 };
	 X x2 = { s0 };
	 if( x0 != x1) cout << "bug: x0 != x1" << endl;
     if( x0 != x2) cout << "bug: x0 != x2" << endl;
	 T_END;
}

void
UniformInitializationSyntax::
memberArrayInit_0()
{
	T_START;
	// declare a class with a member that is an array of std::string
	 class A
	 {
	 public:
		 // C++11 only; initialize members with brackets inside a class
		 A():m_desc{"A0","A1","A2","A3"} {};
		 void toString()
		 {
			 for(int i = 0; i < 4; i++ )
			 {
				 std::cout << m_desc[i] << std::endl;
			 }
		 };
     private:
		 std::string m_desc[4];
	 };
	 A a;
	 a.toString();
	 T_END;
}

// Same as memberArrayInit_0() but use std::array as a member.
void
UniformInitializationSyntax::
memberArrayInit_1()
{
	T_START;
	// declare a class with a member that is an array of std::array
	 class A
	 {
	 public:
		 // C++11 only; initialize std::array members with brackets inside an initializer list.
		 A (): m_desc{"A0","A1","A2","A3"} {};
		 void toString()
		 {
			 for( auto const &n: m_desc)
			 {
				 std::cout << n << std::endl;
			 }
		 };
     private:
		 std::array<std::string,4> m_desc;
	 };
	 // invoke constructor to test.
	 A a;
	 a.toString();
	 T_END;
}

// in C++11 containers can be initialized with brackets.
void
UniformInitializationSyntax::
containerIntialization()
{
	 T_START;
	class A
	{
	public:
	    A():m_arrayInt{1,2,3,4,5,6} {};
	    A(const A& a):m_arrayInt(a.m_arrayInt),m_arrayChar(a.m_arrayChar) {};
	    A& operator=(const A& a)
	    {
	    	if(this == &a) return *this;
	    	this->m_arrayInt = a.m_arrayInt;
	    	this->m_arrayChar = a.m_arrayChar;
	    	return *this;
	    }

	    std::string toString() const
	    {
	        std::stringstream s;
	        for( auto const &n : m_arrayInt)
	        {
	            s << " " << n;
	        }
	        s << "\n";
	        for( auto &n : m_arrayChar)
	        {
	        	s << " " << n;
	        }
	        return s.str();
	    }

	private:
		std::array<int,6> m_arrayInt;
		// I have no idea why double  brackets are required....
		// C++11 only; initialize containers via brackets
		std::array<char,5> m_arrayChar={{'a','b','c','d','e'}};

	};

    std::vector<A> vector_a { A(), A() };
    int i =0;
    for ( auto const &v : vector_a)
    {
         std::cout << "object A[i]=" << i++ << "\n" << v.toString() << std::endl;
    }
    T_END;

}

// in C++11, members can be initialized in the class declaration
void
UniformInitializationSyntax::
inClassInitialization()
{
	T_START;
	class A
	{
	public:
		std::string toString() const
		{
			return m_string;
		}
	private:
		// C++11 only
		std::string m_string = "laurence";
	};
	A a;
	cout << a.toString() << endl;
	T_END;
}

} // end of namespace UniformInit


/*
C c {0,0}; //C++11 only. Equivalent to: C c(0,0);

int* a = new int[3] { 1, 2, 0 }; /C++11 only

class X {
  int a[4];
public:
  X() : a{1,2,3,4} {} //C++11, member array initializer
};
With respect to containers, you can say goodbye to a long list of push_back() calls. In C++11 you can initialize containers intuitively:

// C++11 container initializer
vector<string> vs={ "first", "second", "third"};
map singers =
  { {"Lady Gaga", "+1 (212) 555-7890"},
    {"Beyonce Knowles", "+1 (212) 555-0987"}};
Similarly, C++11 supports in-class initialization of data members:

class C
{
 int a=7; //C++11 only
public:
 C();
};

*/


