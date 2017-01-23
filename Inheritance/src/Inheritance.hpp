/*
 * Inheritance.hpp
 *
 *  Created on: Dec 8, 2016
 *      Author: USER
 */

#ifndef INHERITANCE_HPP_
#define INHERITANCE_HPP_

#include <cstddef>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

#pragma once

using namespace std;

#define T_START cout << "Start**  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << endl;
#define T_END cout <<    "End  **  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << endl;


// This section exercises constructors, and copy constructors, inheritance and overloaded operators.
class B
{
public:
	inline B():m_b(" Class B ") {T_START};
	inline B(const std::string& s):m_b(s) {T_START;T_END};
	inline ~B() {T_START;T_END};
	inline B(const B& b):m_b(b.m_b) {T_START};
	inline B& operator=(const B& lhs)
	{
		if( this == &lhs) return(*this);
		this->m_b = lhs.m_b;
		return *this;
	}
	// implement the == operator; the implementation below is the same as the default implementation
	inline friend bool operator==(const B& lhs, const B& rhs) {return (lhs.m_b == rhs.m_b);};
	inline std::string toString() { return m_b;}
private:
	std::string m_b;
};

class A1
{
public:
	inline A1(const char* a1);
	inline A1(const std::string s);
	inline virtual ~A1();
	inline A1(const A1& a1);
	inline A1& operator=(const A1& a1);
	inline friend bool operator==(const A1& lhs, const A1& rhs);
	inline virtual std::string toString() const {return std::string(m_a1);};
protected:
	char* m_a1;
	A1();
};

A1::A1(const char* a1)
{
	T_START;
	m_a1 = new char[sizeof a1];
	strcpy(m_a1,a1);
	T_END;
};
A1::A1(const std::string s)
{
	T_START;
	m_a1 = new char(s.size());
	strcpy(m_a1,s.c_str());
	T_END;
}
A1::~A1() { T_START;if(m_a1) delete m_a1; T_END};
A1::A1(const A1& a1)
{
	T_START;
	m_a1 = new char(sizeof a1.m_a1);
	strcpy(m_a1,a1.m_a1);
	T_END;
}
A1& A1::operator=(const A1& a1)
{
	T_START;
	if(this != &a1)
	{
	    if(m_a1) delete m_a1;
	    m_a1 = new char[sizeof(a1.m_a1)];
	    strcpy(m_a1,a1.m_a1);
	}
	T_END;
	return *this;
}

bool operator==(const A1& lhs, const A1& rhs)
{ return(! (strcmp(lhs.m_a1,rhs.m_a1))); };

class A:A1
{
public:
   inline A(const std::string& s, const B& b, const std::string& a1 );
   inline A(const A& a);
   inline ~A();
   inline A& operator=(const A& a);
   inline friend bool operator==(const A& lhs, const A& rhs);
   inline friend bool operator!=(const A& lhs, const A& rhs) { return ( lhs == rhs);};
   inline std::string toString() const;
private:
   A();
   std::string m_a;
   B*  m_b;
};

A::A(const std::string& s, const B& b, const std::string& a1 ):A1(a1),m_a(s)
{
	T_START;
	m_b = new B(b);
	T_END;
}
A::A(const A& a):A1(a),m_a(a.m_a)
{
	T_START;
	m_b = new B(*(a.m_b));
	T_END;
};
A::~A()
{
	  T_START;
	  if(m_b) delete m_b;
	  T_END;
}

A& A::operator=(const A& a)
{
	T_START;
    if( this != &a)
    {
	    m_a = a.m_a;
	    m_a1 = a.m_a1;
		if( m_b) delete m_b;
		m_b = new B(*(a.m_b));
    }
	T_END;
    return *this;
}
inline bool operator==(const A& lhs, const A& rhs)
{  return ((lhs.m_a == rhs.m_a) && (!strcmp(lhs.m_a1,rhs.m_a1)) && (*(lhs.m_b) == *(rhs.m_b)));};

std::string A::toString() const
{
	stringstream ss;
	ss << "A::m_a=" << m_a.c_str() << " A::m_b=" << m_b->toString() << " A:A1::m_a1=" << A1::toString() << endl;
	return ss.str();
}



// The classes below illustrate the "diamond problem".
// To fix, use the virtual base class
class Mammal
{
public:
	enum Gender
	{
		male   = 0,
		female = 1,
		last_item = 2
	};

	inline Mammal(Gender gender):m_gender(gender)
	{
		T_START;
		cout << "Mammal(" << Mammal::m_genderMap.at(gender) <<  ")" << endl;
		T_END
	};

	inline const std::string&& toString() const
	{
		// return a temporary.  In C++11, the item will stay in scope until it is released.
		return ( "Mammal is " + m_genderMap.at(m_gender) );
	}

	inline const char* toStringEnum() const
	{
		return (m_array[m_gender]);
	}

	inline virtual Gender sex() { return  m_gender;};
	inline virtual ~Mammal() {};

	// create string representations of the enum in two different ways; make public
    static const std::map<Mammal::Gender, std::string> m_genderMap;
    static const std::array<const char*,2> m_array;

private:
	Gender m_gender;
};




// define classes below to explore inheritance.
class Carnivore:virtual public Mammal
{
public:
	Carnivore(Gender gender, std::string prey):Mammal(gender),m_prey(prey)
    {
		T_START;
		cout << "Carnivore(" << m_genderMap.at(gender) << "prey=" << m_prey << ")" << endl;
		T_END
    };
	std::string prey() { return m_prey;};
	virtual ~Carnivore() {};
private:
	std::string m_prey;
};

class Herbivore:virtual Mammal
{
public:
	Herbivore(Gender gender, std::string vegetable):Mammal(gender),m_vegetable(vegetable)
    {
		T_START;
        cout << "Herbivore(" << m_genderMap.at(gender) <<  ", vegetable=" << vegetable << ")" << endl;
        T_END
    };
	std::string vegetable() { return m_vegetable;};
	virtual ~Herbivore() {}
private:
	std::string m_vegetable;
};


// Note  that if Carnivore and Herbivore do not inherit virtual Mammal, we get the diamond problem.
// e.g. two copies of m_gender..
// The compiler catches is with the error:
// virtual 06:26: error: 'Mammal' is an ambiguous base of 'Bear'
class Bear:public Carnivore, public Herbivore
{
public:
	Bear(Gender gender, std::string prey, std::string vegetable):
		 Carnivore(gender, prey),Herbivore(gender,vegetable),    // Carnivore, Herbivore have no  effect due to virtual
	     Mammal(gender)
    {
		T_START;
        cout << "Bear(" << m_genderMap.at(sex()) << " prey=" << prey << " vegetable=" << vegetable << ")" << endl;
		T_END
    };
	std:: string toString()
	{
		stringstream ss;
		ss << "Bear(" << m_genderMap.at(sex()) << " prey=" << prey().c_str() << " vegetable=" << vegetable().c_str() << ")" << endl;
		return ss.str();
	}

};

#endif /* INHERITANCE_HPP_ */
