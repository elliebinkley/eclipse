/*
 * Anagram.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: USER
 */
/*
Strings: Making Anagrams
by amititkgp
Problem
Submissions
Leaderboard
Discussions
Editorial
puzzle from "Cracking the Coding Interview".
Alice is taking a cryptography class and finding anagrams to be very useful.
We consider two strings to be anagrams of each other if the first string's letters can be rearranged to form the second string.
In other words, both strings must contain the same exact letters in the same exact frequenc.
For example, bacdc and dcbac are anagrams, but bacdc and dcbad are not.
Alice decides on an encryption scheme involving two large strings where encryption is dependent on the
minimum number of character deletions required to make the two strings anagrams. Can you help her find this number?

Given two strings that may or may not be of the same length, determine the minimum number of character deletions required to make
an anagrams. Any characters can be deleted from either of the strings.

This challenge is also available in the following translations:

Chinese
Russian
Input Format:
The first line contains a single string, .
The second line contains a single string, .

Constraints:
It is guaranteed that  and  consist of lowercase English alphabetic letters (i.e.,  through ).
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
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <array>
#include <ctype.h>
#include <stdio.h>

using namespace std;

struct LetterCounter
{
   int  occurrencesA = 0;
   int  occurrencesB = 0;
};
int number_needed( string a, string b, std::stringstream& ss ) {
     int numDeletions = 0;
     std::array<LetterCounter,26> letterArray;

     for( auto &letter:a)
     {
         if( !isalpha(letter)) continue;
         if( !islower(letter)) continue;
         int index = (int)letter - (int)'a';
         letterArray[index].occurrencesA++;
     }
     for( auto &letter:b)
     {
         if( !isalpha(letter)) continue;
         if( !islower(letter)) continue;
         int index = (int)letter - (int)'a';
         letterArray[index].occurrencesB++;
     }
     for( int i=0; i < 26; i++)
     {
	 char letter = 'a' + i;
	 for( int k =0; k < (min(letterArray[i].occurrencesA, letterArray[i].occurrencesB)); k++ )
	 {
	     ss << letter;
	 }
         numDeletions = abs(  letterArray[i].occurrencesA - letterArray[i].occurrencesB) + numDeletions;
     }
     return numDeletions;
}

int anagram()
{
    cout << "starting anagram test:\n"
	 << "determine the minimum number of character deletions required to make anagrams from two strings:\n "
	 <<  "Enter string 1:" << endl;
    std::stringstream ss;
    string a;
    cin >> a;
    cout << "Enter string 2:" << endl;
    string b;
    cin >> b;
    cout << "Number of letters needed to delete = " << number_needed(a, b, ss ) << endl;
    cout << " anagram = \"" << ss.str() << "\"" << endl;
    return 0;
}
