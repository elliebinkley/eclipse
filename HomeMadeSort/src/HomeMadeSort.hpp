/*
 * HomeMadeSort.hpp
 *
 *  Created on: Jan 24, 2017
 *      Author: USER
 *      Define a generic class called "A" that can be used toinvetigate the
 *      std::sort functionality.
 */

#ifndef HOMEMADESORT_HPP_
#define HOMEMADESORT_HPP_

#include <iostream>

#define T_START cout << "Start**  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << endl;
#define T_END cout <<    "End  **  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << endl;

  class A
  {
  public:
    inline A() = default;
    inline A (const std::string& firstName, const std::string lastName) :
	m_firstName (firstName), m_lastName (lastName)   {};
    inline A (const A& a) = default;
    inline A& operator= (const A& a)
    {
      if(this == &a) return *this;
      this->m_firstName = a.m_firstName;
      this->m_lastName  = a.m_lastName;
      return *this;
    }
    inline friend bool operator== (const A& lhs, const A& rhs)
    {
      return ((lhs.m_firstName == rhs.m_firstName )
	    && (lhs.m_lastName == rhs.m_lastName ));
    }
    inline friend bool operator!= (const A& lhs, const A& rhs)
    {
       return !(lhs==rhs);
    }
    inline friend bool operator< (const A& lhs, const A& rhs)
    {
      if (lhs.m_lastName == rhs.m_lastName)
	return( lhs.m_firstName < rhs.m_firstName );
      else
	return ( lhs.m_lastName < rhs.m_lastName );
    }
    // default move = operator
    inline A& operator= (A&& a)
    {
      m_firstName = std::move (a.m_firstName);
      m_lastName = std::move (a.m_lastName);
      return *this;
    }

    // move constructor
    inline A(A&& a): m_firstName(std::move(a.m_firstName)), m_lastName(std::move(a.m_lastName)) {};
    inline ~A () = default;
    void print() const
    {
      std::cout << "Name="<< m_firstName << " " << m_lastName << std::endl;
    }
  private:
    std::string m_firstName;
    std::string m_lastName;
  };


#endif /* HOMEMADESORT_HPP_ */
