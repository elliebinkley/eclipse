/*
 * TestClasses.hpp
 *
 *  Created on: Jan 3, 2017
 *      Author: USER
 */

#ifndef UNIFORMINITIALIZATIONSYNTAX_HPP_
#define UNIFORMINITIALIZATIONSYNTAX_HPP_

#include <iostream>

namespace UniformInit
{

// make a singleton class and use it to run all the tests.
  class UniformInitializationSyntax
  {
  public:
    // get singleton
    static UniformInitializationSyntax* getInstance();
    void runTests();
    void arrayInit();
    void memberArrayInit_0();
    void memberArrayInit_1();
    void containerIntialization();
    void inClassInitialization();
  private:
    // make private the constructor
    UniformInitializationSyntax()
    {};

    // make private the copy constructor
    UniformInitializationSyntax( const UniformInitializationSyntax& object )
    {};

    // make private the =  operator
    UniformInitializationSyntax&
    operator= (const UniformInitializationSyntax& object)
    {
      return *this;
    };

    static UniformInitializationSyntax* uniformInitializationSyntaxPtr; // declare the ptr but define in .cpp file.
  };

  class X
  {
  public:
    X (std::string s) :
	m_x (s)
    {};

    X (const X& x) : m_x (x.m_x)
    {};

    virtual
    ~X ()
    {};

    X& operator= (const X& rhs)
    {
      if (this == &rhs ) return *this;
      this->m_x = rhs.m_x;
      return (*this);
    }

    friend bool operator== (const X& lhs, const X& rhs)
    {
      return (lhs.m_x == rhs.m_x);
    };

    friend bool operator!= (const X& lhs, const X& rhs)
    {
      return (lhs.m_x != rhs.m_x);
    };

  private:
    std::string m_x;
  };

} // end of name space

#endif /* UNIFORMINITIALIZATIONSYNTAX_HPP_ */
