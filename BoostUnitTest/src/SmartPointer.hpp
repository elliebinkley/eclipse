/*
 * SmartPointer.hpp
 *
 *  Created on: Jan 16, 2017
 *
 *      Author: Burley
 */

#ifndef SMARTPOINTER_HPP_
#define SMARTPOINTER_HPP_

#include <vector>
#include <iostream>
#include <sstream>
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
      m_ptr_string = std::unique_ptr<std::string> (new string (s));
     // std::cout << __FUNCTION__ << "(" << s << ")" << std::endl;
    }

    inline
    ~A()
    {
      //std::cout << __FUNCTION__ << std::endl;
    }

    inline A( const A& a )
    {
      // copy the string from the old pointer member to the new pointer member
      // without disturbing the unique_pointer.
      m_ptr_string = std::unique_ptr<std::string> ( new string (*(a.m_ptr_string.get ())));
    }

    // move constructor; need to supply as user defined constructors/destructors are supplied which invalidates default
    // move constructor supplied by compiler.
    inline A (A&& o) noexcept : m_ptr_string(std::move(o.m_ptr_string))
    {
    	//  std::cout << "A(A&&): move constructor invoked\n";
    }

    inline std::string toString() const
    {
      stringstream ss;
	  if(m_ptr_string.get() == nullptr)   // nullptr new for C++11...
	  {
	    ss << __FUNCTION__<< " :m_string_unique_ptr= nullptr" << endl;
	  }
	  else
	  {
	    ss << __FUNCTION__<< " :m_string_unique_ptr=" << *m_ptr_string.get() << endl;
	  }
	  return ss.str();
    }

    inline bool isValid()
    {
        if( m_ptr_string ) return true;
        return( false );
    }

    std::unique_ptr<std::string> m_ptr_string { (new std::string("unknown"))};
  };

} // end namespace SmartPtr
#endif /* SMARTPOINTER_HPP_ */

