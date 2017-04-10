/*
 * TestClasses.hpp
 *
 *  Created on: Jan 3, 2017
 *      Author: USER
 */

#ifndef UNIFORMINITIALIZATIONSYNTAXTEST_HPP_
#define UNIFORMINITIALIZATIONSYNTAXTEST_HPP_

#include <iostream>
#include <sstream>

namespace UniformInit
{

  class A
  {
  public:
      A (const int value, const std::string& description) : m_a (value), m_desc (description)  {};
      std::string toString() const
      {
          std::stringstream s;
          s << m_desc.c_str() << ":m_a=" << m_a << std::endl;
          return s.str ();
      };
      int getVal() const { return m_a; };
      const std::string& getDesc() const { return m_desc; };
  private:
      int m_a;
      std::string m_desc;
  };

  // declare and test class called X
  class X
  {
  public:
    X (std::string s)  : m_x (s)  {};
    X (const X& x) : m_x (x.m_x)  {};
    virtual ~X ()  {};
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

#endif /* UNIFORMINITIALIZATIONSYNTAXTEST_HPP_ */
