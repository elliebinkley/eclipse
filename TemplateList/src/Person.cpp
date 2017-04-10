/*
 * Person .cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: USER
 */

#include "Person.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include "stdio.h"
#include "stdlib.h"

Person::Person( const std::string& firstName, const std::string& lastName,
                const std::string& address, const SSN& ssn ) :
        m_firstName( firstName ), m_lastName( lastName ), m_address( address ), m_ssn( ssn )
{
}

Person::Person( const char* firstName, const char* lastName, const char* address, const SSN* ssn ) :
        m_firstName( firstName ), m_lastName( lastName ), m_address( address ), m_ssn( * ssn )
{
}
Person::Person( const std::string* firstName, const std::string* lastName,
                const std::string* address, const SSN* ssn ) :
        m_firstName( * firstName ), m_lastName( * lastName ), m_address( * address ), m_ssn( * ssn )
{
}

Person::Person( const Person& person ) :
        m_firstName( person.m_firstName ), m_lastName( person.m_lastName ), m_address(
                person.m_address ), m_ssn( person.m_ssn )
{
}

Person::Person() :
        m_firstName( NULL ), m_lastName( NULL ), m_address( NULL ), m_ssn( 0, 0, 0 )
{
}

Person::~Person()
{
}

void Person::print()
{
    cout << "name:" << m_firstName << " " << m_lastName << " ssn:" << m_ssn.getSSN() << " address:"
            << m_address << endl;
}

bool operator==( const Person& p1, const Person& p2 )
{
    return (p1.m_firstName == p2.m_firstName && p1.m_lastName == p2.m_lastName
            && p1.m_address == p2.m_address && p1.m_ssn == p2.m_ssn);
}

bool operator!=( const Person& p1, const Person& p2 )
{
    return ! (p1 == p2);
}

SSN::SSN( int firstPart, int secondPart, int thirdPart ) :
        m_firstPart( firstPart ), m_secondPart( secondPart ), m_thirdPart( thirdPart )
{
}

SSN::SSN( const std::string ssn ) :
        m_firstPart( 0 ), m_secondPart( 0 ), m_thirdPart( 0 )
{
    int length = ssn.length();
    if( length != SSN_LENGTH ) return;

    const char* firstString = ssn.c_str();
    bool isValid = true;

    for( int i = 0; i < length; i++, firstString++ )
    {
        const char c = firstString[i];
        if( ! isdigit( c ) ) isValid = false;
    }
    if( ! isValid ) return;

    m_firstPart = atoi( ssn.substr( 0, 2 ).c_str() );
    m_secondPart = atoi( ssn.substr( 3, 4 ).c_str() );
    m_thirdPart = atoi( ssn.substr( 5, 8 ).c_str() );
}

SSN::~SSN()
{
}

std::string SSN::getSSN() const
{
    char buf[3][5];
    sprintf( buf[0], "%d", m_firstPart );
    sprintf( buf[1], "%d", m_secondPart );
    sprintf( buf[2], "%d", m_thirdPart );
    std::string ssn( buf[0] );
    ssn.append( "-" );
    ssn.append( buf[1] );
    ssn.append( "-" );
    ssn.append( buf[2] );
    return ssn;
}

bool operator==( const SSN& ssn1, const SSN& ssn2 )
{
    return (ssn1.m_firstPart == ssn2.m_firstPart && ssn1.m_secondPart == ssn2.m_secondPart
            && ssn1.m_thirdPart == ssn2.m_thirdPart);
}

bool operator!=( const SSN& ssn1, const SSN& ssn2 )
{
    return ! (ssn1 == ssn2);
}

