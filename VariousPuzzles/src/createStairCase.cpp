/*
 * createStairCase.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: USER
 */
#include <iostream>
#include <string>
#include <iterator>
#include "stdio.h"
#include "string.h"
#include "Utilities.hpp"


using namespace std;

// given a number N; create a staircase pattern e.g.
//      x
//     xx
//    xxx
//   xxxx
//  xxxxx
// xxxxxx
// in this case N is 6.
void createStairCase()
{
    T_START;
    int N = 15;
    cout <<  "given a number N; create a staircase pattern e.g. \n"
            "      x  \n"
            "     xx  \n"
            "    xxx  \n"
            "   xxxx  \n"
            "  xxxxx  \n"
            " xxxxxx  \n"
            " in this case N is 6 " << endl;

    cout << "\n\nMethod 1; Using C, N=" << N << endl;
    char temp[N + 1];
    memset( temp, ' ' , N );
    temp[N]=0;
    for( int i = 0; i < N; i++ )
    {
        temp[ N-i ] = 'x';
        printf( "%s\n", temp );
    }
    cout << "Method 1 End\n" << endl;

    cout << "Method 2; Using C++; N=" << N << endl;
    string s(N, ' ');
    for(auto rIter = s.rbegin(); rIter != s.rend(); rIter++ )
    {
       *rIter = 'x';
        cout << s << endl;
    }
    cout << "Method 2 End\n" << endl;
    cout << "SUCCESS" << endl;

    T_END;
}

