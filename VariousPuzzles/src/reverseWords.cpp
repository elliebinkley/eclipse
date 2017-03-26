#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"

#include "Utilities.hpp"

extern bool checkInput( const char* input );

// Given an array of characters which form a sentence of words, write an efficient algorithm to
// reverse the order of the words (not characters) in it.
void reverseWords()
{
    T_START

    std::cout
            << "Test: Given an array of characters which form a sentence of words, write an efficient algorithm to "
            << "reverse the order of the words (not characters) in it." << std::endl;
    const char* phrase = "this is an absolute mess   why are you here today";
    if( !checkInput( phrase ) )
    {
        T_END
        return;
    }

    // Method 1
    printf( "Method 1; using c; using strtok\n" );
    int length = strlen( phrase ) + 100;
    char c_phrase[length];
    char temp[length] = { 0 };
    char result[length] = { 0 };

    strncpy( c_phrase, phrase, length );

    char* token = strtok( c_phrase, " " );
    while( token != NULL )
    {
        strncpy( temp, result, length );
        sprintf( result, "%s %s", token, temp );
        token = strtok( NULL, " " );
    }
    printf( "input=\"%s\" \noutput=\"%s\"\n", phrase, result );
    printf( "End Method1\n" );

    // Method 2
    std::cout << "Method 2: using STL string.find() and string.insert() techniques" << std::endl;
    std::string s( phrase );
    std::string result1;
    size_t start = 0;
    size_t end = 0;
    // walk through the string, finding the words, then insert the word substring at the beginning of the result string
    while( (end = s.find( " ", start )) <= s.length() )
    {
        std::string sub = s.substr( start, (end - start) + 1 );
        // protect against multiple blanks.
        if( sub != " " )
        {
            result1.insert( 0, s.substr( start, (end - start) + 1 ) );
        }
        start = ++end;
    }
    // if s does not end with a " ", then get the last string
    if( end == s.npos )
    {
        std::string last = s.substr( start, (s.length() - start) + 1 ) + " ";
        result1.insert( 0, last );
    }
    std::cout << "input=\"" << s << "\"\noutput=\"" << result1 << "\"" << std::endl;
    std::cout << "End Method 2" << std::endl;

    // Method 3
    std::cout
            << "Method 3: using stringstream to find words, put them into a vector and a reverse iterate on the vector "
            << std::endl;
    std::stringstream ss( phrase );
    std::vector<std::string> vec;
    std::string buffer;
    while( ss >> buffer )
    {
        vec.push_back( buffer );
    }
    std::vector<std::string>::reverse_iterator iter;

    std::cout << "input=\"" << phrase << "\"\noutput=\"";
    for( iter = vec.rbegin(); iter != vec.rend(); iter++ )
    {
        std::cout << * iter << " ";
    }
    std::cout << "\"" << std::endl;
    std::cout << "End Method 3" << std::endl;

    std::cout << "SUCCESS" << std::endl;

    T_END
}

// utility method
bool checkInput( const char* input )
{
    std::string checkInput( input );
    if( checkInput.length() == 0 )
    {
        std::cout << "Input Failure:\"" << checkInput << "\" is of zero length; skipping test"
                << std::endl;
        return false;
    }
    bool foundChar = false;
    for( auto &c : checkInput )
    {
        if( isalpha( c ) )
        {
            foundChar = true;
        }
        else if( c != ' ' )
        {
            std::cout << "Input Failure:\"" << checkInput
                    << "\" contains non letter characters=\'" << c << "\'; skipping test" << std::endl;
            return false;
        }
    }
    return foundChar;
}
