/*
 * SmartPointer.hpp
 *
 *  Created on: Jan 16, 2017
 *      Author: USER
 */

#ifndef SMARTPOINTER_HPP_
#define SMARTPOINTER_HPP_

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <memory>

using namespace std;
namespace SmartPtr
{

  class A
  {
  public:
    inline A() = default;
    inline A(const std::string& s)
    {
      m_ptr_string = std::unique_ptr<std::string> (new string ("->" + s));
      std::cout << __FUNCTION__ << "(" << s << ")" << std::endl;
    }

    inline
    ~A()
    {
      std::cout << __FUNCTION__ << std::endl;
    }

    inline A( const A& a )
    {
      // copy the string from the old pointer member to the new pointer member
      // without disturbing the unique_pointer.
      m_ptr_string = std::unique_ptr<std::string> ( new string (*(a.m_ptr_string.get ())));
    }

    // move constructor; need to supply as user defined constructors/destructors are supplied.
    inline A (A&& o) noexcept : m_ptr_string(std::move(o.m_ptr_string))
    {
    	std::cout << "A(A&&): move constructor invoked\n";
    }

    inline void print() const
    {
	  if(m_ptr_string.get() == nullptr)
	  {
	    cout << __FUNCTION__<< " :m_string_unique_ptr= nullptr" << endl;
	  }
	  else
	  {
	    cout << __FUNCTION__<< " :m_string_unique_ptr=" << *m_ptr_string.get() << endl;
	  }
    }

  private:
    std::unique_ptr<std::string> m_ptr_string { (new std::string("unknown"))};
  };

  class SmartPointer
  {
  public:
    static SmartPointer* getInstance (void);
    void runTests ();
    void smartPointerTest_1 ();
    void smartPointerTest_2 ();
    void smartPointerTest_3 ();
    void smartPointerTest_4 ();
    void smartPointerTest_5 ();
  private:
    inline SmartPointer() = default;
    inline SmartPointer(  const SmartPointer & ) = delete;
    inline SmartPointer& operator= (SmartPointer &&) = delete;
    inline SmartPointer& operator= (const SmartPointer &) = delete;
    inline friend bool operator== (const SmartPointer& lhs, const SmartPointer& rhs)
    {
      return false;
    };
    inline ~SmartPointer()
    {
      if (m_smartPointer)
	  delete m_smartPointer;
    };

//	std::vector<A> m_vector { A("a1"), A("a2"), A("a3") };
    static SmartPointer* m_smartPointer;

  };

} // end namespace SmartPtr
#endif /* SMARTPOINTER_HPP_ */
