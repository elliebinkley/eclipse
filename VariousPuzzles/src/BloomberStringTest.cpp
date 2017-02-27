/*
 * BloombergStringTest.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: USER
 */

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include "stdio.h"
#include "string.h"
#include "Utilities.hpp"

/* given a string, delete all duplicate characters and prefix the first instance of the
 * character with the number of occurrences duplicates
 * The  input is standard input
 * hellotheworld -> 2h2e3l2otwrd
 */

using namespace std;

extern std::string getInput();
extern void bloombergStringTestVersion1(std::string input );
extern void bloombergStringTestVersion2(std::string input );

void bloombergStringTest()
{
    T_START
    std::cout
               << "Test: given a string, delete all duplicate characters and prefix the first instance of the \n"
               << "character with the number of occurrences duplicates\n"
               << "eg. hellotheworld -> 2h2e3l2otwrd \n  " << std::endl;
    std::string input = getInput();
    bloombergStringTestVersion1(input);
    bloombergStringTestVersion2(input);
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
    std::cout << "input  =\"" << input << "\"" << std::endl;
    return input;
}

void bloombergStringTestVersion1(std::string input )
{
    // version 1
    T_START
    cout << "Method 1" << endl;

    int length = input.size();
    char inputStr[length + 1] { };
    char outputStr[2 * length] { };

    int outIdx = 0;

    strncpy( inputStr, input.c_str(), length );

    for( int i = 0; i < length; i++ )
    {
        // char must be between a and z.
        if( ! isalpha( (char) inputStr[i] ) ) continue;
        if( ! islower( (char) inputStr[i] ) ) continue;

        int numDups = 0;
        for( int k = i + 1; k < length; k++ )
        {
            if( inputStr[i] == inputStr[k] )
            {
                inputStr[k] = '?'; // placeholder so it gets skipped.
                numDups++;
            }
        }

        if( numDups )
        {
            char numberStr[5];
            sprintf( numberStr, "%d", numDups + 1 );
            strcat( outputStr, numberStr );
            outIdx += strlen( numberStr );
        }
        outputStr[outIdx] = inputStr[i];
        outIdx++;
    }
    std::cout << "output =\"" << outputStr << "\"" <<  std::endl;
    cout << "Method 1 End" << endl;
    T_END

}

void bloombergStringTestVersion2(std::string input )
{
    T_START
    cout << "Method 2" << endl;

    // find out how many times a letter has occurred and record this in the occurrence map.
    // if a duplicate is detected in the input string, replace it with a "?"
    std::map<char, int> occurrenceMap;
    std::string::iterator iter;
    for( iter = input.begin(); iter != input.end(); iter++ )
    {
        if( ! isalpha( (char) (* iter) ) ) continue;
        if( ! islower( (char) (* iter) ) ) continue;
        // insertion into a map will return false as an map pair entry if the key exists already
        // for the pair being inserted.
        // false is returned as a std::pair with the first part of the pair an iterator to the found
        // element pair.
        std::pair<std::map<char, int>::iterator, bool> ret;
        ret = occurrenceMap.insert( std::pair<char, int>( * iter, 1 ) );
        if( ret.second == false ) // already in array
        {
            // replace the duplicate with a "?"  in the original input string
            * iter = '?';
            // increment the occurrence count in the map
            ret.first->second++;
        }
    }

    // walk through the input string and print out the letters and their occurrence count if > 1.
    std::cout << "output =\"";
    map<const char, int>::iterator findMapValue;
    // walk through the input string and print out the characters that are not a "?, along with the count..
    for( const auto &iter1 : input )
    {

        if( ! isalpha( (char) (iter1) ) ) continue;
        if( ! islower( (char) (iter1) ) ) continue;
        findMapValue = occurrenceMap.find( (char) (iter1) );
        if( findMapValue != occurrenceMap.end() )
        {
            int numOccurrence = occurrenceMap[iter1];
            if( numOccurrence > 1 )
            {
                cout << numOccurrence;
            }
            cout << (char) (iter1);
        } else
        {
            // value not found ion the occurrence map.. bug.
            cout << "should not happen" << endl;
        }
    }
    cout << "\"" << endl;
    cout << "Method 2 End" << endl;
    T_END

}

