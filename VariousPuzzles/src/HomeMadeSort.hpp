/*
 * HomeMadeSort.hpp

#include <iostream>

 *
 *  Created on: Jan 24, 2017
 *      Author: USER
 *      Define a generic class called "A" that can be used toinvetigate the
 *      std::sort functionality.
 */

#ifndef HOMEMADESORT_HPP_
#define HOMEMADESORT_HPP_

  class NameList
  {
  public:
    inline NameList() = default;
    inline NameList (const std::string& firstName, const std::string lastName) :
	                 m_firstName (firstName), m_lastName (lastName)   {};
    inline NameList (const NameList& a) = default;
    inline NameList& operator= (const NameList& a)
    {
      if(this == &a) return *this;
      this->m_firstName = a.m_firstName;
      this->m_lastName  = a.m_lastName;
      return *this;
    }
    inline friend bool operator== (const NameList& lhs, const NameList& rhs)
    {
      return ((lhs.m_firstName == rhs.m_firstName )
	           && (lhs.m_lastName == rhs.m_lastName ));
    }
    inline friend bool operator!= (const NameList& lhs, const NameList& rhs)
    {
       return !(lhs==rhs);
    }
    inline friend bool operator< (const NameList& lhs, const NameList& rhs)
    {
      if (lhs.m_lastName == rhs.m_lastName)
	       return( lhs.m_firstName < rhs.m_firstName );
      else
	       return ( lhs.m_lastName < rhs.m_lastName );
    }
    // default move = operator
    inline NameList& operator= (NameList&& a)
    {
      m_firstName = std::move (a.m_firstName);
      m_lastName = std::move (a.m_lastName);
      return *this;
    }

    // move constructor
    inline NameList(NameList&& a): m_firstName(std::move(a.m_firstName)), m_lastName(std::move(a.m_lastName)) {};
    inline ~NameList () = default;
    void print() const
    {
      std::cout << m_firstName << ":" << m_lastName << " ";
    }

  private:
    std::string m_firstName;
    std::string m_lastName;
  };


#endif /* HOMEMADESORT_HPP_ */
