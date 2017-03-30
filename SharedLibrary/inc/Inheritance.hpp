/*
 * Inheritance.hpp
 *
 *  Created on: Dec 8, 2016
 *  Author: Larry Burley
 */

#ifndef INC_INHERITANCE_HPP_
#define INC_INHERITANCE_HPP_

#include <cstddef>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include "Utilities.hpp"

using namespace std;
namespace Inheritance
{

// This section exercises constructors, and copy constructors, inheritance and overloaded operators.
class B
{
public:
    inline B() : m_b( " Class B " )  {}
    inline B( const std::string& s ) : m_b( s )  {}
    inline ~B() {}
    inline B( const B& b ) : m_b( b.m_b ) {}
    inline B& operator=( const B& lhs )
    {
        if( this == & lhs )
            return (* this);
        this->m_b = lhs.m_b;
        return * this;
    }
    // implement the == operator; the implementation below is the same as the default implementation
    inline friend bool operator==( const B& lhs, const B& rhs )    { return (lhs.m_b == rhs.m_b); }
    inline std::string toString()   { return m_b; }
private:
    std::string m_b;
};

class A1
{
public:
    inline A1( const char* a1 );
    inline A1( const std::string s );
    inline virtual ~A1();
    inline A1( const A1& a1 );
    inline A1& operator=( const A1& a1 );
    inline friend bool operator==( const A1& lhs, const A1& rhs );
    inline virtual std::string toString() const  { return std::string( m_a1 ); }
    inline std::string toStringNonVirt() const  { return std::string( m_a1 ); }
protected:
    char* m_a1;
    A1();
};

A1::A1( const char* a1 )
{
    m_a1 = new char[sizeof a1];
    strcpy( m_a1, a1 );
}

A1::A1( const std::string s )
{
    m_a1 = new char( s.size() );
    strcpy( m_a1, s.c_str() );
}

A1::~A1()
{
    if( m_a1 )   delete m_a1;
}

A1::A1( const A1& a1 )
{
    m_a1 = new char( sizeof a1.m_a1 );
    strcpy( m_a1, a1.m_a1 );
}
A1& A1::operator=( const A1& a1 )
{
    if( this != & a1 )
    {
        if( m_a1 )
            delete m_a1;
        m_a1 = new char[sizeof(a1.m_a1)];
        strcpy( m_a1, a1.m_a1 );
    }
    return * this;
}

bool operator==( const A1& lhs, const A1& rhs )  { return (! (strcmp( lhs.m_a1, rhs.m_a1 ))); }

class A: public A1
{
public:
    inline A( const std::string& s, const B& b, const std::string& a1 );
    inline A( const A& a );
    inline ~A();
    inline A&  operator=( const A& a );
    inline friend bool operator==( const A& lhs, const A& rhs );
    inline friend bool operator!=( const A& lhs, const A& rhs ) { return (lhs == rhs); }
    inline virtual std::string  toString() const override;
    inline std::string toStringNonVirt() const;
private:
    A();
    std::string m_a;
    B* m_b;
};

A::A( const std::string& s, const B& b, const std::string& a1 ) : A1( a1 ), m_a( s )
{
    m_b = new B( b );
}

A::A( const A& a ) : A1( a ), m_a( a.m_a )
{
    m_b = new B( *(a.m_b) );
}

A::~A()
{
    if( m_b )
        delete m_b;
}

A& A::operator=( const A& a )
{
    if( this != &a )
    {
        m_a = a.m_a;
        m_a1 = a.m_a1;
        if( m_b )
            delete m_b;
        m_b = new B( * (a.m_b) );
    }
    return * this;
}

inline bool operator==( const A& lhs, const A& rhs )
{
    return ((lhs.m_a == rhs.m_a) && (! strcmp( lhs.m_a1, rhs.m_a1 ))
            && (* (lhs.m_b) == * (rhs.m_b)));
}

std::string A::toString() const
{
    stringstream ss;
    ss << "A::m_a=" << m_a.c_str() << " A::m_b=" << m_b->toString()
            << " A:A1::m_a1=" << A1::toString() << endl;
    return ss.str();
}

std::string A::toStringNonVirt() const
{
    stringstream ss;
    ss << "A::m_a=" << m_a.c_str() << " A::m_b=" << m_b->toString()
            << " A:A1::m_a1=" << A1::toString() << endl;
    return ss.str();
}

// The classes below illustrate the "diamond problem".
// To fix, use the virtual base class on the derived classes
class Mammal
{
public:
    enum Gender
    {
        male = 0, female = 1, last_item = 2
    };

    inline Mammal( Gender gender ) : m_gender( gender )
    {
        // cout << "Mammal(" << Mammal::m_genderMap.at( gender ) << ")" << endl;
    }

    inline const std::string&& toString() const
    {
        // return a temporary.  In C++11, the item will stay in scope until it is released.
        return ("Mammal is " + m_genderMap.at( m_gender ));
    }

    inline const char* toStringEnum() const
    {
        return (m_array[m_gender]);
    }

    inline virtual Gender sex() const
    {
        return m_gender;
    }

    inline virtual ~Mammal() {}

    static const char* getEnumValue(Mammal::Gender gender);

    // create string representations of the enum in two different ways; make public
    static const std::map<Mammal::Gender, std::string> m_genderMap;
    static const std::array<const char*, 2> m_array;

private:
    Gender m_gender;
};

// define classes below to explore inheritance.
// virtual is to prevent the diamond problem
class Carnivore: virtual public Mammal
{
public:
    Carnivore( Gender gender, std::string prey ) : Mammal( gender ), m_prey( prey )
    {
      //  cout << "Carnivore(" << m_genderMap.at( gender ) << " prey=" << m_prey << ")" << endl;
    }

    std::string prey() const
    {
        return m_prey;
    }

    virtual ~Carnivore() {}
private:
    std::string m_prey;
};

// virtual is to prevent the diamond problem
class Herbivore: virtual public Mammal
{
public:
    Herbivore( Gender gender, std::string vegetable ) : Mammal( gender ), m_vegetable( vegetable )
    {
      //  cout << "Herbivore(" << m_genderMap.at( gender ) << ", vegetable="<< vegetable << ")" << endl;
    }

    std::string vegetable() const
    {
        return m_vegetable;
    }

    virtual ~Herbivore() {}

private:
    std::string m_vegetable;
};

// Note  that if Carnivore and Herbivore inherit from Mammal, we get the diamond problem.
// e.g. two copies of m_gender..
// The compiler catches is with the error:
// virtual 06:26: error: 'Mammal' is an ambiguous base of 'Bear'
// To fix this, make Mammal a virtual class in Herbivore and Carnivore declaration and then in the constructor,
// initialize base class Mammal.
class Bear: public Carnivore, public Herbivore
{
public:
    Bear( Gender gender, std::string prey, std::string vegetable ) :
            Carnivore( gender, prey ), Herbivore( gender, vegetable ), // Carnivore, Herbivore have no  effect due to virtual
            Mammal( gender )
    {
      //  cout << "Bear(" << Mammal:: m_genderMap.at( sex() ) << " prey=" << prey << " vegetable=" << vegetable << ")" << endl;
    }

    std::string toString() const
    {
        stringstream ss;
        ss << "Bear(" << m_genderMap.at( sex() ) << " prey=" << prey().c_str()
                << " vegetable=" << vegetable().c_str() << ")" << endl;
        return ss.str();
    }
};

} // end of namespace Inheritance

#endif /* INC_INHERITANCE_HPP_ */
