//============================================================================
// Name        : Inheritance.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Inheritance.hpp"

using namespace std;

extern void testOut(void);


// stringify the Gender enumeration using Map.
// Bracket initialization is a C++11 feature.
// Initialization of static must be in a .cpp file
const std::map<Mammal::Gender, std::string> Mammal::m_genderMap =
{
		{Mammal::Gender::male,"male"},
		{Mammal::Gender::female,"female"}
};

// stringify the Gender enumeration using constexpr on std::array, which tells the compiler to
// initialized the m_array structure.  Only works on "literal" structures/classes/primitives.
// constexpr is a C++11 feature.
// Initialization of static must be in a .cpp file
#define stringify( name ) # name   // # name returns the string "name" ; see definitions of # in preprocessor
constexpr std::array<const char*, Mammal::Gender::last_item> Mammal::m_array =
{
	{
		stringify( male ),
		stringify( female )
	}
};

// stringify the Gender enumeration using preprocessor.
// This technique below does the same thing as the std::map<Gender, std::string> genderMap above in terms of
// defining enumerations strings, but
// does it with the preprocessor, therefore it is faster since the stringify() does not occur at boot time, but
// at compile time.
const char* GenderString[] =
{
  stringify( male ),
  stringify( female )
};

const char*
getEnumValue(Mammal::Gender x)
{
	 return GenderString[x];
}

int main() {
	T_START;
	{
		A1 test_a1("whatever");
		A1 test_a2(std::string("whateverXX"));
		A1 test_a3(test_a2);
		std::cout << "test A1 (ctor) = " << (( test_a3 == test_a2) ? "true" : "false") << std::endl;
		test_a3 = test_a1;
		std::cout << "test A1 (==) = " << (( test_a3 == test_a1) ? "true" : "false") << std::endl;
	}

	std::string a(" A::string ");
	std::string a1(" A1::char* ");
	B b(" B::string");
	{
		A a_class(a, b, a1);
	}

	{
		A a_class(a, b, a1);
		A a_class_1(a, b, a1);
		std::cout << "test1: (==) = " << (( a_class_1 == a_class)  ? "true" : "false") << std::endl;
	}
	{
		A a_class_1(a, b, a1);
		A a_class = a_class_1;
		std::cout << "test2: ( = ) = " << (( a_class_1 == a_class)  ? "true" : "false") << std::endl;
	}
	{
		A a_class_1(a, b, a1);
		A a_class_2(a_class_1);
		std::cout << "test3: ( cp ctor ) = " << (( a_class_2 == a_class_1)  ? "true" : "false") << std::endl;
	}
	{
		A a_class(a, b, a1);
		cout << "test4: A::toString()" <<  a_class.toString() << endl;
	}
	{
		// The diamond problem and how to solve it.
		// See virtual keyword Carnivore/Herbivore class declarations
		Bear b1(Mammal::Gender::male, "elk", "berries");
	    cout << "Bear::B1 is " << b1.toString() << endl;

	    Bear b2(Mammal::Gender::female,"elk", "berries");
	    cout << "Bear b2 is " << b2.toString() << endl;
	}
	{
	   // how to stringify enumerations: 3 ways.
	   // a. using the preprocessor
	   std::cout << "enum= " << Mammal::Gender::female  <<"=" << getEnumValue(Mammal::Gender::female) << std::endl;
	   std::cout << "enum= " << Mammal::Gender::male  <<"=" << getEnumValue(Mammal::Gender::male) << std::endl;

	   // b. using the constexpr to initialize a static class
	   std::cout << "m_array[" << Mammal::Gender::male  <<"]=" << Mammal::m_array[Mammal::Gender::male] << std::endl;

	   // c. stringify enumerations using a static map class.
	   // Note that m_genderMap[Mammal::female] cannot be used, since m_genderMap is declared as a const and "[]" will insert
	   // an element if the element does not exist, implying a modification to a const class, whcih the compiler objects to.
	   //  at() does not insert, only reads..
		std::cout << "m_genderMap[ " << Mammal::Gender::female << "]=" << Mammal::m_genderMap.at(Mammal::Gender::female) << std::endl;
	}
	{
		// investigate std::literal_type classes.   Note that only literal type classes can use
		// bracket initialization. So how do you know which classes are literal so you can/cannot use
		// bracket initialization; use std::literal to find out..
		std:: cout << "\n" << endl;
		std::cout << "Bear: " << std::is_literal_type<Bear>::value << std::endl;  // true
		std::cout << "std::map: "   << std::is_literal_type<std::map<int, int>>::value << std::endl; // false
		std::cout << "std::array: " << std::is_literal_type<std::array<const char*,20>>::value << std::endl; // true
		std::cout << "int: "        << std::is_literal_type<int>::value << std::endl;  // true
    }

	T_END;
	return 0;
}






