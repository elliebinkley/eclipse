/*
 * BloombergStringTest.cpp
 *
 *  Created on: Nov 22, 2016
 *  Modified    2020/02/06
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

class Letter
{
public:
    Letter(char letter, int numDups ): m_letter(letter), m_numDups(numDups) {};

    bool operator() ( const Letter& item ) const
    {
        bool ret= ( m_letter < item.m_letter );
        //cout << "< " << ret << " m_letter=" << m_letter << " item.m_letter" << item.m_letter << endl;
        return ret;
    }

    bool operator== ( const Letter& item ) const
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
    struct hash<Letter>
    {
        size_t
        operator()(const Letter & obj) const
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


/* use an unorderer_set to keep track of letters and how many times they appeared.
 * use a queue to remember how they are ordered
 * have the queue and the unordered_set share Letter via shared pointers.
 */
void bloombergStringTestVersion1(const std::string& input )
{
    T_START

    unordered_set<shared_ptr<Letter>,Deref::Hash, Deref::Compare> s2;
    queue<shared_ptr<Letter>>  qRec;

    for( const auto& iter : input )
    {
        shared_ptr lRec = make_shared<Letter>(iter,1);
        auto pair = s2.insert(lRec);
        if( !(pair.second ))
        {
            (*pair.first)->m_numDups++;   // already present, increment dup count
        }
        else
        {
            qRec.push(lRec);
        }
    }
    cout << "\"";
    while( !qRec.empty() )
    {
        shared_ptr<Letter> item = qRec.front();
        qRec.pop();
        if ( item->m_numDups != 1 ) cout << item->m_numDups;
        cout << item->m_letter;
    }
    cout << "\"" << endl;
}


