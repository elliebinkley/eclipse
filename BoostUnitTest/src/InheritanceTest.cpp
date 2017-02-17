/*
 * InheritanceTest.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: Burley
 */

#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <cstdlib>
#include <vector>
#include <SharedLibrary/inc/MyLogger.hpp>
#include <SharedLibrary/inc/Inheritance.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace Inheritance;

BOOST_AUTO_TEST_SUITE( Inheritance )
BOOST_AUTO_TEST_CASE( A1_CopyAndAssignmentTest )
{
    A1 test_a1( "whatever" );
    A1 test_a2( std::string( "whateverXX" ) );
    A1 test_a3( test_a2 );
    BOOST_CHECK( test_a3 == test_a2 );
    test_a3 = test_a1;
    BOOST_CHECK( test_a3 == test_a1 );
}

BOOST_AUTO_TEST_CASE( A_CopyAndAssignmentTest )
{
  std::string a_string (" A::string ");
  std::string a1_string (" A1::char* ");
  B b_string (" B::string");

  // test ==
  A a_class_1( a_string, b_string, a1_string );
  A a_class_2( a_string, b_string, a1_string );
  BOOST_CHECK(a_class_1 == a_class_2 );

  // test assignment
  A a_class_3 = a_class_1;
  BOOST_CHECK(a_class_3 == a_class_1 );

  // test ctor
  A a_class_4 (a_string, b_string, a1_string);
  A a_class_5 (a_class_1);
  BOOST_CHECK(a_class_5 == a_class_4 );
}

BOOST_AUTO_TEST_CASE( A_toStringTest )
{
    std::string a_string (" A::string ");
    std::string a1_string (" A1::char* ");
    B b_string (" B::string");

    A a_class_1( a_string, b_string, a1_string );
    BOOST_CHECK(a_class_1.toString().c_str() != nullptr );
}

BOOST_AUTO_TEST_CASE( DiamondTest )
{
    // The diamond problem and how to solve it.
    // See virtual keyword Carnivore/Herbivore class declarations
    Bear b1 (Mammal::Gender::male, "elk", "berries");
    BOOST_CHECK(b1.toString().c_str() != nullptr );

    Bear b2 (Mammal::Gender::female, "elk", "berries");
    BOOST_CHECK(b2.toString().c_str() != nullptr );
}

BOOST_AUTO_TEST_CASE( EnumStringify )
{
    string s;
    string s1 = "female";
    string s2 = "male";

    // how to stringify enumerations: 3 ways.
    // a. using the preprocessor
    // b. test that getEnumValue() works....
    s = Mammal::getEnumValue (Mammal::Gender::female);
    BOOST_TEST( s == s1);

    s = Mammal::getEnumValue (Mammal::Gender::male);
    BOOST_TEST( s == s2);

    // b. using the constexpr to initialize a static class
    s = Mammal::m_array[Mammal::Gender::female];
    BOOST_TEST( s == s1);

    s =   Mammal::m_array[Mammal::Gender::male];
    BOOST_TEST( s == s2);

    // c. stringify enumerations using a static map class.
    // Note that m_genderMap[Mammal::female] cannot be used, since m_genderMap is declared as a const and "[]" will insert
    // an element if the element does not exist, implying a modification to a const class, whcih the compiler objects to.
    //  at() does not insert, only reads..
    s =  Mammal::m_genderMap.at (Mammal::Gender::female);
    BOOST_TEST( s == s1);

    s =  Mammal::m_genderMap.at (Mammal::Gender::male);
    BOOST_TEST( s == s2);
}

BOOST_AUTO_TEST_CASE( LiteralType )
{
    // investigate std::literal_type classes.
    // Note that only literal type classes can use bracket initialization.
    // So how do you know which classes are literal? use std::literal to find out..
    BOOST_TEST( !((bool) (std::is_literal_type<Bear>::value) ));                // Not literal ..
    BOOST_TEST( !((bool) (std::is_literal_type<std::map<int, int>>::value) ));  // literal ..
    BOOST_TEST(   (bool) (std::is_literal_type<std::array<const char*, 20>>::value) );
    BOOST_TEST(   (bool) (std::is_literal_type<std::array<const char*, 20>>::value) );
    BOOST_TEST(   (bool) (std::is_literal_type<int>::value ));
}
BOOST_AUTO_TEST_SUITE_END()


