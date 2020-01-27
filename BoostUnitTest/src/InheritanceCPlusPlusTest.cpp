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
#include <BoostUnitTest/src/InheritanceCPlusPlus.hpp>
#include <UtilitiesLib/inc/MyLogger.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace InheritanceCPlusPlus;

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


// toString is virtualized. What happens when base class pointer toString() is called?
BOOST_AUTO_TEST_CASE( A_toStringTestVirtual )
{
    // test to see if toString on class A works.
    std::string a_string( " A::string " );
    std::string a1_string( " A1::char* " );
    std::string b_string( " B::string" );
    B b( b_string );

    A* a_class = new A( a_string, b, a1_string );

    // see if formulated string is correct.
    // Class A's string?

    std::size_t found;

    found = a_class->toString().find( a_string );
    BOOST_CHECK( found != std::string::npos );

    // Base Class A1's string?
    found = a_class->toString().find( a1_string );
    BOOST_CHECK( found != std::string::npos );

    // Class B's string?
    found = a_class->toString().find( b_string );
    BOOST_CHECK( found != std::string::npos );

    // cast class pointer to base class and rerun the identical checks. They should pass.
    A1* a1_class = a_class;
    // Class A's string?
    found = a1_class->toString().find( a_string );
    BOOST_CHECK( found != std::string::npos );

    // Class A1's string?
    found = a1_class->toString().find( a1_string );
    BOOST_CHECK( found != std::string::npos );

    // Class B's string?
    found = a1_class->toString().find( b_string );
    BOOST_CHECK( found != std::string::npos );
}

// toStringNonVirt() is not virtualized. What happens when base class pointer toString() is called?
//  no-polymorphism..
BOOST_AUTO_TEST_CASE( A_toStringTestNonVirtual )
{
    // test to see if toStringNonVirt on class A works.
    std::string a_string (" A::string ");
    std::string a1_string (" A1::char* ");
    std::string b_string(" B::string");
    B b(b_string);

    A* a_class = new A( a_string, b, a1_string );

    // cout << "a_class.toStringNonVirt()=" << a_class->toStringNonVirt() << endl;
    // cout << "a_class.toString()=" << a_class->toString() << endl;

    // see if formulated string is correct.
    // Class A's string?
    std::size_t found;

    found = a_class->toStringNonVirt().find(a_string);
    BOOST_CHECK( found != std::string::npos);

    // Class A1's string?
    found = a_class->toStringNonVirt().find(a1_string);
    BOOST_CHECK( found != std::string::npos);

    // Class B's string?
    found = a_class->toStringNonVirt().find(b_string);
    BOOST_CHECK( found != std::string::npos);

    // cast class pointer to base class.
    A1* a1_class = a_class;

    //     cout << "a1_class.toStringNonVirt()=" << a1_class->toStringNonVirt() << endl;
    //     cout << "a1_class.toStringNonVirt()=" << a1_class->toString() << endl;
    // see if formulated toStringNonVirt is correct.  Should call A1's toStringNonVirt(); e.g. no  polymorphism...
    // Class A's string?
    found = a1_class->toStringNonVirt().find(a_string);
    BOOST_CHECK( found == std::string::npos);

    // Class A1's string?
    found = a1_class->toStringNonVirt().find(a1_string);
    BOOST_CHECK( found != std::string::npos);

    // Class B's string?
    found = a1_class->toStringNonVirt().find(b_string);
    BOOST_CHECK( found == std::string::npos);
}


//  show dynamic casting.
//  no-polymorphism..
BOOST_AUTO_TEST_CASE( A_DynamicCast )
{
    std::string a_string (" A::string ");
    std::string a1_string (" A1::char* ");
    std::string b_string(" B::string");
    B b(b_string);

    A* a_class = new A( a_string, b, a1_string );
    A1* a1_class = a_class;                          // impliciltly cast class pointer down to base class; compiler check
                                                     //     or
    A1* another_a1_class = static_cast <A1*> (a_class); // static cast class pointer down to base class; compiler check

    A* anotherA = dynamic_cast <A*>(a1_class);       // dynamic cast up to derived class;runtime check

    // see if formulated string is correct.
    // Class A's string?
    std::size_t found;

    found = anotherA->toStringNonVirt().find(a_string);
    BOOST_CHECK( found != std::string::npos);

    // Class A1's string?
    found = anotherA->toStringNonVirt().find(a1_string);
    BOOST_CHECK( found != std::string::npos);

    // Class B's string?
    found = anotherA->toStringNonVirt().find(b_string);
    BOOST_CHECK( found != std::string::npos);

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


