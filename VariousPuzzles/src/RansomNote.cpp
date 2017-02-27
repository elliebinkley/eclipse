/*
 * RansomNote.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: USER
 *
 * Hash Tables: Ransom Note ( A HackerRank Puzzle.. )
 * by saikiran9194
 * Problem
 * Submissions
 * Leaderboard
 * Discussions
 * Editorial
 * Check out the resources on the page's right side to learn more about hashing. The video tutorial is by
 * Gayle Laakmann McDowell, author of the best-selling interview book Cracking the Coding Interview.
 * A kidnapper wrote a ransom note but is worried it will be traced back to him.
 * He found a magazine and wants to know if he can cut out whole words from it and use them to create an
 * untraceable replica of his ransom note. The words in his note are case-sensitive and he must
 * use whole words available in the magazine, meaning he cannot use substrings or concatenation to
 * create the words he needs.
 * Given the words in the magazine and the words in the ransom note, print Yes if he can replicate his ransom
 * note exactly using whole words from the magazine; otherwise, print No.
 *
 *  Input Format
 *  The first line contains two space-separated integers describing the respective values of
 *  (the number of words in the magazine) and  (the number of words in the ransom note).
 *  The second line contains  space-separated strings denoting the words present in the magazine.
 *  The third line contains  space-separated strings denoting the words present in the ransom note.
 *  Constraints
 *      Each word consists of English alphabetic letters (i.e.,  to  and  to ).
 *      The words in the note and magazine are case-sensitive.
 *  Output Format
 *      Print Yes if he can use the magazine to create an untraceable replica of his ransom note; otherwise, print No.
 *  Sample Input
 *      6 4
 *      give me one grand today night
 *      give one grand today
 *  Sample Output
 *      Yes
 *  Explanation
 *  All four words needed to write an untraceable replica of the ransom note are present in the magazine,
 *  so we print Yes as our answer.
 *
 *
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

using namespace std;

bool ransom_note( vector<string> magazine, vector<string> ransom )
{
    map<string, int> map_magazine;
    map<string, int> map_ransom;
    // construct magazine map
    for( auto &s : magazine )
    {
        auto x = map_magazine.find( s ); // x is going to be a std::pair<std::map<const string,int>::iterator, bool>
        if( x == map_magazine.end() )
        {
            // use auto to insert
            map_magazine[s] = 1;
        } else
        {
            x->second++;    // increment occurrence count in the map
        }
    }
    for( auto &s : ransom )
    {
        auto x = map_ransom.find( s ); // x is going to be a std::pair<std::map<const string,int>::iterator, bool>
        if( x == map_ransom.end() )
        {
            // use auto to insert
            map_ransom[s] = 1;
        } else
        {
            x->second++;    // increment occurrence count.
        }
    }
    for( auto &s : map_ransom )
    {
        auto x = map_magazine.find( s.first );
        if( x == map_magazine.end() ) return false;  //
        if( x->second < s.second ) return false;
    }
    return true;
}

void ransomNotePuzzle()
{
    int m;
    int n;
    cout << "  Enter two space-separated integers describing the respective values of  "
            "  (the number of words in the magazine) and  (the number of words in the ransom note)."
            " e.g. \"6 4 \""
            << endl;
    cin >> m >> n;
    cout << "   Enter space-separated strings denoting the words present in the magazine."
            "   the number of words should match the first number entered above."
            " e.g. \"give me one grand today night|\""
               << endl;
    vector<string> magazine( m );
    for( int magazine_i = 0; magazine_i < m; magazine_i++ )
    {
        cin >> magazine[magazine_i];
    }
    cout << "  Enter space-separated strings denoting the words present in the planned ransom note."
            "  The number of words should match the second number entered above."
            "  \"give one grand today\" "
                   << endl;
    vector<string> ransom( n );
    for( int ransom_i = 0; ransom_i < n; ransom_i++ )
    {
        cin >> ransom[ransom_i];
 //       cout << "ransom_i= " << ransom_i << endl;
    }
    if( ransom_note( magazine, ransom ) )
        cout << "Yes, the ransom note can be written with the words provided\n";
    else cout << "No, the ransome note cannot be written with the words provided \n";
    return;
}

