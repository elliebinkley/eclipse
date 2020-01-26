/*
 * Person.h
 *
 *  Created on: Nov 17, 2016
 *      Author: L. Burley
 */

#ifndef INC_PERSON_HPP_
#define INC_PERSON_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include "Printable.hpp"

using namespace std;

#define SSN_LENGTH 9

class SSN
{
public:
   SSN( const std::string ssn ):
      m_firstPart( 0 ), m_secondPart( 0 ), m_thirdPart( 0 )
   {
       int length = ssn.length();
       if( length != SSN_LENGTH )
       {
          std::cerr << "invalid SSN" << ssn <<  " string length:" << length <<  std::endl;
          return;
       }

       const char* tmp = ssn.c_str();
       bool isValid = true;

       for( int i = 0; i < length; i++, tmp++ )
       {
           const char c = *tmp;
           if( ! isdigit( c ) ) isValid = false;
       }
       if( ! isValid )
       {
          std::cerr << "invalid SSN string contents not all digits:" << ssn<< std::endl;
          return;
       }

       m_firstPart  = atoi( ssn.substr( 0, 3 ).c_str() );
       m_secondPart = atoi( ssn.substr( 3, 2 ).c_str() );
       m_thirdPart  = atoi( ssn.substr( 5, 8 ).c_str() );
   }

   SSN( int firstPart, int secondPart, int thirdPart ) :
           m_firstPart( firstPart ), m_secondPart( secondPart ), m_thirdPart( thirdPart ) {}
   std::string getSSN() const
   {
   	  stringstream ss;
      ss << m_firstPart << "-" << m_secondPart <<  "-" << m_thirdPart;
      return ss.str();
   }

   virtual ~SSN() = default;

   friend bool operator==( const SSN& ssn1, const SSN& ssn2 )
   { return (ssn1.m_firstPart == ssn2.m_firstPart && ssn1.m_secondPart == ssn2.m_secondPart
               && ssn1.m_thirdPart == ssn2.m_thirdPart);
   }

   friend bool operator!=( const SSN& ssn1, const SSN& ssn2 )
   {  return ! (ssn1 == ssn2);   }

   friend bool operator<( const SSN& ssn1, const SSN& ssn2 )
   {
       if ( ssn1.m_firstPart < ssn2.m_firstPart)  return true;
       if ( ssn1.m_firstPart > ssn2.m_firstPart)  return false;

       if ( ssn1.m_secondPart < ssn2.m_secondPart) return true;
       if ( ssn1.m_secondPart > ssn2.m_secondPart) return false;

       if ( ssn1.m_thirdPart < ssn2.m_thirdPart) return true;
       if ( ssn1.m_thirdPart > ssn2.m_thirdPart) return false;

       return false;
   }

   friend bool operator<=( const SSN& ssn1, const SSN& ssn2 )
   {
	   if  ( ssn1 < ssn2 )  return true;
	   if ( ssn1 == ssn2 )  return true;
	   return false;
   }

   friend bool operator>( const SSN& ssn1, const SSN& ssn2 )
   {
	   if ( ssn1 <= ssn2 )   return false;
	   return true;
   }

   friend bool operator>=( const SSN& ssn1, const SSN& ssn2 )
   {
      if ( ssn1 < ssn2 ) return false;
      return true;
   }

private:
   SSN();  // make private; need to construct with values....
   int m_firstPart;
   int m_secondPart;
   int m_thirdPart;

};

class Person : public Printable
{
public:
   Person( const Person& person ) = default;
   Person( const std::string& firstName, const std::string& lastName,
                   const std::string& address, const SSN& ssn ) :
           m_firstName( firstName ), m_lastName( lastName ),
		   m_address( address ), m_ssn( ssn ) {};

   Person( const char* firstName, const char* lastName, const char* address, const SSN* ssn ) :
           m_firstName( firstName ), m_lastName( lastName ),
		   m_address( address ), m_ssn( * ssn )
   { };

   Person( const std::string* firstName, const std::string* lastName,
                   const std::string* address, const SSN* ssn ) :
           m_firstName( * firstName ), m_lastName( * lastName ),
		   m_address( * address ), m_ssn( * ssn )
   { };

   virtual ~Person() = default;

   inline std::string getFirstName() const { return m_firstName; }
   inline std::string getLastName() const  { return m_lastName;  }
   inline std::string getAddress() const   { return m_address;   }
   inline void setFirstName( const std::string firstName ) { m_firstName = firstName;}
   inline void setLastName( const std::string lastName ) { m_lastName = lastName; }
   inline void setAddress( const std::string address ) { m_address = address; }
   inline void setFirstName( const char* const firstName ) { m_firstName = firstName; }
   inline void setLastName( const char* const lastName )  {  m_lastName = lastName; }
   inline void setAddress( const char* const address )  {  m_address = address;  }
   inline SSN getSSN() const  {  return m_ssn;  }


   bool operator() ( const Person& lhs, const Person& rhs )
        { return ( lhs.getSSN() < rhs.getSSN()); }
   friend bool operator==( const Person &p1, const Person &p2 )
		{ return ( p1.getSSN() == p2.getSSN() ); }
   friend bool operator!=( const Person &p1, const Person &p2 )
		{ return ( p1.getSSN() != p2.getSSN() ); }
   friend bool operator<( const Person &p1, const Person &p2 )
        { return ( ( p1.getSSN() < p2.getSSN() ) ); }
   friend bool operator<=( const Person &p1, const Person &p2 )
           { return ( ( p1.getSSN() <= p2.getSSN() ) ); }
   friend bool operator>( const Person &p1, const Person &p2 )
        { return ( ( p1.getSSN() > p2.getSSN() ) ); }
   friend bool operator>=( const Person &p1, const Person &p2 )
           { return ( ( p1.getSSN() >= p2.getSSN() ) ); }

   friend std::size_t hash_value(const Person &value)
   {
	   cout << "Person::hash_value() on " << value.getSSN().getSSN() << endl;
	   return ( std::hash<std::string>{}(value.getSSN().getSSN() ) );
   }

   void print() const
   {
       cout << "name:" << m_firstName << " " << m_lastName << " ssn:" << m_ssn.getSSN() << " address:"
               << m_address << std::endl;
   }

   void print( std::ostream& s ) const
   {
      s << "name:" << m_firstName << " " << m_lastName << " ssn:" << m_ssn.getSSN() << " address:"
            << m_address << std::endl;
   }



private:
   Person();  // make private; can't construct without values...
   Person& operator=( const Person &rhs );
   std::string m_firstName;
   std::string m_lastName;
   std::string m_address;
   SSN m_ssn;
};




#endif /* INC_PERSON_HPP_ */
