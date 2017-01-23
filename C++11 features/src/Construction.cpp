/*
 * Construction.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: Burley
 *      Brief: Investigate various ways to construct a class.
 *      Description:
 *      1. copy constructor,
 *      2. assignment operator,
 *      3. move construction ; this is new for C++11.
 *      4. default constructor; ; this is new for C++11.
 *      5. move assignment  ; this is new for C++11.
 *      6. default constructor ; this is new for C++11.
 *      7. default destructor ; ; this is new for C++11.
 */


#include <iostream>
#include <sstream>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include "Construction.hpp"
#include "Utilities.hpp"

using namespace std;

namespace Construct
{

// Define the static pointer for the singleton in the cpp class and set it to zero;
// Note: do not use static Keyword on definition
Construction* Construction::constructionPtr = nullptr; // nullptr valid as of C++11

// get the singleton instance
Construction*
Construction::getInstance()
{
	 if (constructionPtr == nullptr) // nullptr valid as of C++11
	 {
		 // lazy instatiation... Only do once..
		 constructionPtr = new Construction();
	 }
	 return constructionPtr;
}

// run all tests in this suite
void
Construction::
runTests()
{
	constructionTest_1();
	constructionTest_2();
    return;
}

// Test construction with user defined definitions for class A that implement various construction/assignment/move techniques.
// See http://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html
void
Construction::
constructionTest_1()
{
    T_START;
    class A
	{
	public:
    	// 1. default constructor
    	A() {cout << "A() invoked: default constructor" << endl;};
    	// 2. user supplied constructor
     	A(const std::vector<int>& input ):m_vec(input) {cout << "A(vector<int>&) invoked" << endl;};
     	// 3. copy constructor
     	A(const A& a ):m_vec(a.m_vec) {cout << "A(A&) invoked: copy constructor" << endl;};
     	// 4. copy assignment operator
     	A& operator=(const A& a)
    	{
			if(this == &a ) return(*this);
			this->m_vec = a.m_vec;
			cout << "A a1 = a ; assignment operator= invoked" << endl;
			return(*this);
		};
        // 5. move constructor; need to use std::move on all members. Note all ATL containers support std::move.
        A(A&& o) noexcept : m_vec(std::move(o.m_vec))   { std::cout << "A(A&&): move constructor invoked\n"; };
        // 6. move assignment operator
        A& operator=(A&& other)
		{
			 m_vec = std::move(other.m_vec);
			 std::cout << "move assignment invoked\n";
			 return *this;
		}
        // 7. destructor
		~A() { cout << "~A() invoked" << endl; };

    	std::string toString()
    	{
    		std::stringstream ss;
    		for( auto const &n : m_vec)
    		{
    			ss << n << " ";
    		}
    		return ss.str();
    	};
    	// 8. == operator
    	/* cannot declare friend class in a local class
    	friend bool operator==(const A& rhs, const A& lhs) const
		{
    		return ( rhs.m_vec == lhs.m_vec);
		}
		*/
	private:
    	std::vector<int> m_vec { 10,11,12,13 };  // C++11 ..
	};

    // 1. default constructor.
    A a1;
    cout << "a1:" << a1.toString() << "\n" << endl;

    // 2. user supplied constructor; invalidates the compiler supplied default and copy constructor;
    // so they must be supplied also.
    std::vector<int> j { 20,21,22,23,24,25 };
    A a2(j);
    cout << "a2:" << a2.toString() << "\n" <<  endl;

    // 3. copy constructor
    A a3(a2);
    cout << "a3:" << a3.toString() <<   endl;
    cout << "a2:" << a2.toString() << "\n"  << endl;

    // 3. Copy constructor
    A a4 = a3;
    cout << "a4:" << a4.toString() << endl;
    cout << "a3:" << a3.toString() << "\n" << endl;

    //  4. assignment operator
    a4 = a1;
    cout << "a4:" << a4.toString() << endl;
    cout << "a1:" << a1.toString() << "\n" << endl;

    // 5. move constructor
    A a5 = std::move(a4);
    cout << "a5:" << a5.toString() << endl;
    cout << "a4:" << a4.toString() << "\n" << endl;

    // 6. move assignment operator
    a5 = std::move(a1);
	cout << "a5:" << a5.toString() << endl;
	cout << "a1:" << a1.toString() << "\n" << endl;


    T_END;
}

// Test construction/assignment with compiler generated  definitions for class A
// that implement various construction/assignment idioms.
// Note: if user defined destructor is supplied, then move ctor and move assignment are not generated.
// The compiler generates the move constructor if
//	there is no user-declared copy constructor, and
//	there is no user-declared copy assignment operator, and
//	there is no user-declared move assignment operator and
//	there is no user-declared destructor,
//	it is not marked as deleted,
//	and all members and bases are moveable.
//Similar for the move assignment operator: It is generated if
//	there is no user-declared copy constructor, and
//	there is no user-declared copy assignment operator, and
//	there is no user-declared move constructor and
//	there is no user-declared destructor,
//	it is not marked as deleted,
//	and all members and bases are moveable.

void Construction::constructionTest_2() {
	T_START
	;
	class A {
	public:

		// 1. default constructor
		A() {
			cout << "A() invoked: default constructor" << endl;
		};

		// 2. user supplied constructor
		A(const std::vector<int>& input) :
				m_vec(input)
		{
			cout << "A(vector<int>&) invoked" << endl;
		};
/*  use the compiler supplied version of these classes.
		 // 3. copy constructor
		 A(const A& a ):m_vec(a.m_vec) {cout << "A(A&) invoked: copy constructor" << endl;};
		 // 4. copy assignment operator
		 A& operator=(const A& a)
		 {
		 if(this == &a ) return(*this);
		 this->m_vec = a.m_vec;
		 cout << "A a1 = a ; assignment operator= invoked" << endl;
		 return(*this);
		 };
		 // 5. move constructor
		 A(A&& o) noexcept : m_vec(std::move(o.m_vec))   { std::cout << "A(A&&): move constructor invoked\n"; };
		 // 6. move assignment operator
		 A& operator=(A&& other)
		 {
		 m_vec = std::move(other.m_vec);
		 std::cout << "move assignment invoked\n";
		 return *this;
		 }
*/
/*
		 // 7. destructor; uncomment this and the move ctor and move assignment are no longer generated by compiler.
		 ~A() { cout << "~A() invoked" << endl; };
*/
		 std::string toString()
		 {
		 std::stringstream ss;
		 for( auto const &n : m_vec)
		 {
		 ss << n << " ";
		 }
		 return ss.str();
		 };
		 private:
		 std::vector<int> m_vec { 10,11,12,13 };  // C++11 ..
		 };
		 // 1. default constructor.
		 A a1;
		 cout << "a1:" << a1.toString() << "\n" << endl;

		 // 2. user supplied constructor; invalidates the compiler supplied default and copy constructor;
		 // so they must be supplied also.
		 std::vector<int> j { 20, 21, 22, 23, 24, 25 };
		 A a2(j);
		 cout << "a2:" << a2.toString() << "\n" << endl;

		 // 3. copy constructor
		 A a3(a2);
		 cout << "a3:" << a3.toString() << endl;
		 cout << "a2:" << a2.toString() << "\n" << endl;

		 // 3. Copy constructor
		 A a4 = a3;
		 cout << "a4:" << a4.toString() << endl;
		 cout << "a3:" << a3.toString() << "\n" << endl;

		 //  4. assignment operator
		 a4 = a1;
		 cout << "a4:" << a4.toString() << endl;
		 cout << "a1:" << a1.toString() << "\n" << endl;

		 // 5. move constructor
		 A a5 = std::move(a3);
		 cout << "a5:" << a5.toString() << endl;
		 cout << "a3:" << a3.toString() << "\n" << endl;

		 // 6. move assignment operator
		 a5 = std::move(a1);
		 cout << "a5:" << a5.toString() << endl;
		 cout << "a1:" << a1.toString() << "\n" << endl;

		 T_END;
    }

}  // end namespace Construction



