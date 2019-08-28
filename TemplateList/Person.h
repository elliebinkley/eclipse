/*
 * Person.h
 *
 *  Created on: Nov 17, 2016
 *      Author: L. Burley
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <string>
#include <ostream>
#include "listTemplate.h"

using namespace std;

#define SSN_LENGTH 9

class SSN
{
public:
   SSN( const std::string ssn );
   SSN( int firstPart, int secondPart, int thirdPart );
   std::string getSSN() const;
   virtual ~SSN();

   friend bool operator==( const SSN& ssn1, const SSN& ssn2 );
   friend bool operator!=( const SSN& ssn1, const SSN& ssn2 );
   friend bool operator<( const SSN& ssn1, const SSN& ssn2 );

private:
   SSN();
   int m_firstPart;
   int m_secondPart;
   int m_thirdPart;

};

class Person : public Printable
{
public:
   Person( const Person& person );
   Person( const std::string& firstname, const std::string& lastname, const std::string& address,
           const SSN& ssn );
   Person( const char* firstname, const char* lastname, const char* address, const SSN* ssn );
   Person( const std::string* firstName, const std::string* lastName, const std::string* address,
           const SSN* ssn );
   virtual ~Person();

   inline std::string getFirstName() const
   {
      return m_firstName;
   }
   inline std::string getLastName() const
   {
      return m_lastName;
   }
   inline std::string getAddress() const
   {
      return m_address;
   }
   inline void setFirstName( const std::string firstName )
   {
      m_firstName = firstName;
   }
   inline void setLastName( const std::string lastName )
   {
      m_lastName = lastName;
   }
   inline void setAddress( const std::string address )
   {
      m_address = address;
   }
   inline void setFirstName( const char* const firstName )
   {
      m_firstName = firstName;
   }
   inline void setLastName( const char* const lastName )
   {
      m_lastName = lastName;
   }
   inline void setAddress( const char* const address )
   {
      m_address = address;
   }
   inline SSN getSSN() const
   {
      return m_ssn;
   }

   friend bool operator==( const Person &p1, const Person &p2 );
   friend bool operator!=( const Person &c1, const Person &c2 );
   void print() const;
   void print( std::ostream& s ) const;

private:
   Person();
   Person& operator=( const Person &rhs );
   std::string m_firstName;
   std::string m_lastName;
   std::string m_address;
   SSN m_ssn;
};

#endif /* PERSON_H_ */
