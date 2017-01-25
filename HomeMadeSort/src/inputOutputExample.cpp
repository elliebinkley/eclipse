/*
 * HelloWorld.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: USER
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>

// To complete this challenge, you must save a line of input from stdin to a variable,
// print Hello, World. on a single line, and finally print the value of your variable on a second line.

using namespace std;

extern void
helloWorldCPlusPlus ();
extern void
input_output_with_c_functions ();

void
inputOutputExample ()
{
  helloWorldCPlusPlus ();
  input_output_with_c_functions ();

}

void
input_output_with_c_functions ()
{
  printf ("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

  // Use getChar(), putChar(), gets(), puts(), scanf(), printf(), fopen(), fclose(),
  // fread(), fwrite(), fgetc(), fputc(), getc(), putc()
  // fprintf(), fscanf()
  puts ("Enter input:      ( to stop, enter '.')");
  int c;
  while (((c = getchar ()) != '.'))
    {
      putchar (c);
    }

  printf ("End: %s %s %d\n\n", __FUNCTION__, __FILE__, __LINE__);

}

void
helloWorldCPlusPlus ()
{
  printf ("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
  cout
      << "Using C++; To complete this challenge, you must save a line of input from stdin to a variable"
      << "print Hello, World. on a single line, and finally print the value of your variable on a second line."
      << endl;
  std::string inputString;
  cout << "please type some input and hit ENTER" << endl;
  getline (cin, inputString);

  cout << "Hello World" << endl;
  cout << inputString << endl;
  printf ("End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

}

