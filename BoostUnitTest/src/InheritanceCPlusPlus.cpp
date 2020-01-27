//============================================================================
// Name        : Inheritance.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <cstdlib>
#include <vector>

#include <BoostUnitTest/src/InheritanceCPlusPlus.hpp>
#include <UtilitiesLib/inc/MyLogger.hpp>

using namespace std;
using namespace InheritanceCPlusPlus;

// stringify the Gender enumeration using Map.
// Bracket initialization is a C++11 feature.
// Initialization of static must be in a .cpp file
// Note that this is a const, not a constexpr since map has is not of type literal.
const std::map<Mammal::Gender, std::string> Mammal::m_genderMap = {
        { Mammal::Gender::male, "male" }, { Mammal::Gender::female, "female" } };

// stringify the Gender enumeration using constexpr on std::array, which tells the compiler to
// initialize the m_array structure.  Only works on "literal" structures/classes/primitives like std::array.
// constexpr is a C++11 feature.
// Initialization of static must be in a .cpp file
#define stringify( name ) # name   // # name returns the string "name" ; see definitions of # in preprocessor

constexpr std::array<const char*, Mammal::Gender::last_item> Mammal::m_array =
{ { stringify(
        male ), stringify( female )
} };

// stringify the Gender enumeration using preprocessor.
// This technique below does the same thing as the std::map<Gender, std::string> genderMap above in terms of
// defining enumerations strings, but
// does it with the preprocessor, therefore it is faster since the stringify() does not occur at boot time, but
// at compile time.
const char* GenderString[] = { stringify( male ), stringify( female ) };

const char*
Mammal::getEnumValue( Mammal::Gender x )
{
    return GenderString[x];
}
;

