/*
 * BaseLetter.hpp
 *
 *  Created on: Nov 27, 2019
 *      Author: laurence
 */

#ifndef BASELETTER_HPP_
#define BASELETTER_HPP_

#include <ostream>
#include <string_view>

class BaseLetter
{
public:
  BaseLetter( char c ):m_c( c ) {};
  BaseLetter( BaseLetter& base ) = default;    // Copy CTOR
  BaseLetter( BaseLetter&& base ) = default;   // Move CTOR
  virtual ~BaseLetter()= default;

  inline  char getChar()  const { return m_c; }
  virtual inline const char* getClassName() = 0;

  virtual void print( std::ostream& ios, bool printTree, uint8_t level )  const = 0;
  virtual void validate()  const = 0;


  // relational operations
  bool operator() ( const BaseLetter& lhs, const BaseLetter& rhs )
        { return ( lhs.getChar() < rhs.getChar()); }
  inline bool operator == (const BaseLetter& l) { return (m_c == l.m_c);  }
  inline bool operator <(const BaseLetter& l)   { return (m_c < l.m_c);   }
  inline bool operator <=(const BaseLetter& l)  { return (m_c <= l.m_c);   }
  inline bool operator >(const BaseLetter& l)   { return (m_c > l.m_c);   }
  inline bool operator >=(const BaseLetter& l)  { return (m_c >= l.m_c);   }
  inline bool operator !=(const BaseLetter& l)  { return (m_c != l.m_c ); }

  inline friend bool operator==( const BaseLetter& lhs, const BaseLetter& rhs ) { return (lhs.m_c == rhs.m_c); }
  inline friend std::size_t hash_value(BaseLetter &value)
  {
     return ( std::hash<char>{}( value.getChar() ) );
  }
private:
  char m_c;
  BaseLetter();    // no implementation
};


#endif /* BASELETTER_HPP_ */
