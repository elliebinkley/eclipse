/*
 * Anagram.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: USER
 */
/*
Strings: Making Anagrams
Description: puzzle from "Cracking the Coding Interview".
Alice is taking a cryptography class and finding anagrams to be very useful.
We consider two strings to be anagrams of each other if the first string's letters can be rearranged to form the second string.
In other words, both strings must contain the same exact letters in the same exact frequency.
For example, bacdc and dcbac are anagrams, but bacdc and dcbad are not.
Alice decides on an encryption scheme involving two large strings where encryption is dependent on the
minimum number of character deletions required to make the two strings anagrams. Can you help her find this number?

Given two strings that may or may not be of the same length, determine the minimum number of character deletions required to make
an anagrams. Any characters can be deleted from either of the strings.

Input Format:
The first line contains a single string, .
The second line contains a single string, .

Constraints:
It is guaranteed that  each string consists of lowercase English alphabetic letters (i.e., a through z ).
Output Format:
Print a single integer denoting the number of characters you must delete to make the two strings anagrams of each other.

Sample Input

cde
abc
Sample Output

4
Explanation

We delete the following characters from our two strings to turn them into anagrams of each other:

Remove d and e from cde to get c.
Remove a and b from abc to get c.
We must delete  characters to make both strings anagrams, so we print  on a new line.
 */

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

struct LetterCounter
{
    int  occurrencesStr1 = 0;
    int  occurrencesStr2 = 0;
};

int numDeletionsSolution1( const string& str1, const string& str2 )
{
    const int maxLetters = 26;
    int numDeletions = 0;
    std::array<LetterCounter,maxLetters> letterArray;

    for( auto &letter:str1)
    {
        int index = (int)letter - (int)'a';
        letterArray[index].occurrencesStr1++;
    }
    for( auto &letter:str2)
    {
        int index = (int)letter - (int)'a';
        letterArray[index].occurrencesStr2++;
    }
    for( int i=0; i < maxLetters; i++)
    {
        numDeletions += abs( letterArray[i].occurrencesStr1 - letterArray[i].occurrencesStr2);
    }
    return numDeletions;
}

int numDeletionsSolution2( const string& str1, const string& str2 )
{
    int num_to_delete = 0;
    cout << "find # of characters to delete for strings \"" << str1
            << "\" and \"" << str2 <<"\" to make the strings an anagram" << endl;

    unordered_map<char, LetterCounter> map;
    for( const auto iter : str1 )
    {
        auto pair = map.insert( make_pair(iter,LetterCounter{1,0} ) );
        if ( !pair.second )  pair.first->second.occurrencesStr1++;
    }

    for( const auto iter : str2 )
    {
        auto pair = map.insert( make_pair(iter,LetterCounter{0,1} ) );
        if ( !pair.second ) pair.first->second.occurrencesStr2++;
    }

    for( const auto itMap : map )
    {
        num_to_delete +=  abs ( itMap.second.occurrencesStr1 - itMap.second.occurrencesStr2 );
    }

    return num_to_delete;
}

int anagram()
{
    cout << "starting anagram test:\n"
            << "determine the minimum number of character deletions required to make two strings an anagram of each other:\n "
            <<  "Enter string 1:" << endl;
    std::stringstream ss;
    string str1;
    cin >> str1;
    cout << "Enter string 2:" << endl;
    string str2;
    cin >> str2;

    // make sure all lowercase
    std::array<string, 2> inputs = {str1, str2 };
    for( const auto& iter : inputs )
    {
        for( const auto& item : iter )
        {
            if ( ! ::islower(item) )
            {
                cerr << "bad input \"" << iter << "\"" << endl;
                cerr << "must be lower case" << endl;
                cerr << "stopping anagram test" << endl;
                return -1;
            }
        }
    }
    int sol1 = numDeletionsSolution1(str1, str2 );
    int sol2 = numDeletionsSolution2(str1, str2 );
    cout << "Number of letters needed to delete Solution1 = " << sol1 << endl;
    cout << "Number of letters needed to delete Solution2 = " << sol2 << endl;
    assert ( sol1 == sol2 );
    return 0;
}



