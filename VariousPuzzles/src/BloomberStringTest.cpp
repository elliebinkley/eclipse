/*
 * BloombergStringTest.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: USER
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <queue>
#include <unordered_set>
#include "Utilities.hpp"

/* given a string, delete all duplicate characters and prefix the first instance of the
 * character with the number of occurrences if > 1.
 * The  input is standard input
 * hellotheworld -> 2h2e3l2otwrd
 *
  */

using namespace std;

extern std::string getInput();
extern void bloombergStringTestVersion1(const std::string& input );

class LetterTemp
{
  public:
  LetterTemp(char letter, int numDups ): m_letter(letter), m_numDups(numDups) {};

  bool operator() ( const LetterTemp& item ) const
  {
    bool ret= ( m_letter < item.m_letter );
    //cout << "< " << ret << " m_letter=" << m_letter << " item.m_letter" << item.m_letter << endl;
    return ret;
  }

  bool operator== ( const LetterTemp& item ) const
  {
    bool ret = ( m_letter == item.m_letter );
    //cout << "== " << m_letter << endl;
    return ret;
  }

  char m_letter;
  int  m_numDups;
};

namespace std
{
  template<>
    struct hash<LetterTemp>
    {
      size_t
      operator()(const LetterTemp & obj) const
      {
    // cout << "calling template hash " << obj.m_letter << endl;
        return hash<char>()(obj.m_letter);
      }
    };
}

struct Deref
{
  struct Hash
  {
    template<typename T>
    size_t operator() (shared_ptr<T> const& p) const
    {
      // cout << "calling shared_ptr template hash " << p->m_letter << endl;
      return hash<T>()(*p);
    }
  };
  struct Compare
  {
    template<typename T>
    size_t operator() (shared_ptr<T> const& lhs,
               shared_ptr<T> const& rhs ) const
    {
      // cout << "calling shared_ptr template Compare " << lhs->m_letter << " " <<  rhs->m_letter <<  endl;
      return ( *lhs == *rhs );
    }
  };
};

void bloombergStringTest()
{
    T_START
    std::cout
               << "Test: given a string, delete all duplicate characters and prefix the first instance of the \n"
               << "character with the number of occurrences duplicates\n"
               << "eg. hellotheworld -> 2h2e3l2otwrd \n  " << std::endl;
    std::string input = getInput();
    bloombergStringTestVersion1(input);
    T_END
}

std::string getInput()
{
    std::cout << " Enter input string: " << std::endl;
    std::string input;
    std::cin.clear();
    while ( std::getline( std::cin, input ))
    {
        if( input.size() != 0 ) break;
    }

    for( auto& iter : input )
    {

        // char must be between a and z.
        if( !isalpha( (char) iter ) || !islower( (char) iter ))
        {
            cerr << "isalpha() or islower() failed; bad input=" << iter << endl;
            cerr << "changing to 'a'" << endl;
            iter = 'a';
        }
    }
    cout << "input=" << input << endl;
    return input;
}

void bloombergStringTestVersion1(const std::string& input )
{
   T_START

  unordered_set<shared_ptr<LetterTemp>,Deref::Hash, Deref::Compare> s2;
  queue<shared_ptr<LetterTemp>>  qRecTemp;

  for( const auto& iter : input )
  {
      shared_ptr lRecTemp = make_shared<LetterTemp>(iter,1);
      auto pairTemp = s2.insert(lRecTemp);
      if( !(pairTemp.second ))
      {
         (*pairTemp.first)->m_numDups++;   // already present, increment dup count
      }
       else
      {
         qRecTemp.push(lRecTemp);
      }
  }
  cout << "\"";
  while( !qRecTemp.empty() )
  {
    shared_ptr<LetterTemp> item = qRecTemp.front();
    qRecTemp.pop();
    if ( item->m_numDups != 1 ) cout << item->m_numDups;
    cout << item->m_letter;
  }
  cout << "\"" << endl;
}


