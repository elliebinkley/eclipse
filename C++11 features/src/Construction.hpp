/*
 * Construction.hpp
 *
 *  Created on: Jan 5, 2017
 *      Author: USER
 */

#ifndef CONSTRUCTION_HPP_
#define CONSTRUCTION_HPP_

#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

namespace Construct
{

// make a singleton class and use it to run all the tests.
  class Construction
  {
  public:
    // get singleton
    static Construction* getInstance ();
    void runTests ();
    void constructionTest_1 ();
    void constructionTest_2 ();
  private:
    // make private the constructor; use the default implementation
    Construction () = default;
    // make private the copy constructor;use the default implementation
    Construction (const Construction& object) = default;
    // make private the =  operator;use the default implementation
    Construction&
    operator= (const Construction& object) = default;
    // default does not apply to == operator; so write an implementation; make it private;
    // as a singleton, this should never get called
    friend bool
    operator== (const Construction& rhs, const Construction lhs)
    {
      return (rhs == lhs);
    };
    std::string m_str;
    static Construction* constructionPtr;
  };

// class X uses compiler supplied constructors.
  class X
  {
  public:
    std::string
    toString ()
    {
      std::stringstream ss;
      for (auto const n : m_vec)
	{
	  ss << n << " ";
	}
      return ss.str ();
    }
  private:
    std::vector<int> m_vec
      { 10, 11, 12, 13 };
  };

} // end namespace Construct

#endif /* CONSTRUCTION_HPP_ */
