/*
 *  Simple Inout/Output example from Hacker Rack.
 *
 *  Created on: Nov 22, 2016
 *      Author: lburley
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "Utilities.hpp"

// To complete this challenge, you must save a line of input from stdin to a variable,
// print Hello, World. on a single line, and finally print the value of your variable on a second line.
// see http://www.cplusplus.com/reference/cstdio/ for c library input/output functions.

using namespace std;

extern void
helloWorldCPlusPlus();

extern void
input_output_with_c_functions();

void inputOutputExample()
{
    input_output_with_c_functions();
    helloWorldCPlusPlus();
}

void input_output_with_c_functions()
{
    T_START

    printf("Method 1: Input using C functions\n ");
    puts( "Enter input:      ( to stop, enter '.')" );
    int c;
    while( ((c = getchar()) != '.') )
    {
        // echo to stdout
        putchar( c );
    }
    cout << "Method 1 End" << endl;
    T_END
}

void helloWorldCPlusPlus()
{
    T_START

    cout  << "Various ways to use Input/Output from the console" << endl;


    cout << "Method 2: C++ input using getline(); \n please type some input and hit ENTER\n" << endl;
    cout << " To end; type XXX" << endl;
    std::string inputString;
    //  getline waits for a line feed. cin >> inputString would terminate with a space, but now inputstring has
    // the entire string up to the line feed, spaces included.
    // Note that inputString( passed by reference) is cleared each time through the while loop by getLine.
    while ( getline( cin, inputString ))
    {
        if( inputString == "XXX") break;
        cout << inputString << endl;
    }

    cout << "Method 2 End" << endl;

    // Method 2; general input using cin in a while loop.
    cout << "Method 3: Stream integer input into a vector and then back out;\n"
            "enter some integer data; stop by entering non-integer data e.g. 3 4 23 45 H " << endl;
    std::vector<long> inputInt;
    long i;
    while( cin >> i )
    {
        inputInt.push_back( i );
    }
    if( cin.fail() )
    {
        cin.clear();
        std::string s;
        cin >> s;
        std::cout << "     ( tossing " << s << " .......)" << endl;
    }

    int index = std::distance( inputInt.begin(), inputInt.end() );  // calculates the index.
    cout << "output for " << index << " entries = ";
    for(std::vector<long>::iterator iter = inputInt.begin(); iter < inputInt.end(); iter++ )
    {
        cout <<  * iter << " ";
    }
    cout << endl;
    cout << "Method 3 End" << endl;
    T_END

    return;
}

