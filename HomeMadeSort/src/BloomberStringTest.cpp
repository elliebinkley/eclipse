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
/* given a string, delete all duplicate characters and prefix the first instance of the
 * character with the number of occurrences duplicates
 * The  input is standard input
 * hellotheworld -> 2h2e3l2otwrd
 */

using namespace std;

void
bloombergStringTestVersion1 ()
{
  // version 1
  printf ("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
  std::cout
      << "Test: given a string, delete all duplicate characters and prefix the first instance of the \n"
      << "character with the number of occurrences duplicates\n"
      << "eg. hellotheworld -> 2h2e3l2otwrd \n  " << std::endl;

  std::cout << " Enter input string: " << std::endl;

  std::string input;

  getline (std::cin, input);
  std::cout << "input string = " << input << std::endl;
  int length = input.size ();
  char inputStr[length + 1]
    { };
  char outputStr[2 * length]
    { };

  int outIdx = 0;

  strncpy (inputStr, input.c_str (), length);

  for (int i = 0; i < length; i++)
    {
      // char must be between a and z.
      if (!isalpha ((char) inputStr[i]))
	continue;
      if (!islower ((char) inputStr[i]))
	continue;

      int numDups = 0;
      for (int k = i + 1; k < length; k++)
	{
	  if (inputStr[i] == inputStr[k])
	    {
	      inputStr[k] = '?'; // placeholder so it gets skipped.
	      numDups++;
	    }
	}

      if (numDups)
	{
	  char numberStr[5];
	  sprintf (numberStr, "%d", numDups + 1);
	  strcat (outputStr, numberStr);
	  outIdx += strlen (numberStr);
	}
      outputStr[outIdx] = inputStr[i];
      outIdx++;
    }
  std::cout << "output = " << outputStr << std::endl;
  std::cout << "modified input = " << inputStr << std::endl;

  printf ("End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

}

void
bloombergStringTestVersion2 ()
{

  printf ("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
  std::cout
      << "Test: given a string, delete all duplicate characters and prefix the first instance of the \n"
      << "character with the number of occurrences duplicates\n"
      << "eg. hellotheworld -> 2h2e3l2otwrd \n  " << std::endl;

  std::string input;

  std::cout << " Enter input string: " << std::endl;

  getline (std::cin, input);
  std::cout << "input string = " << input << std::endl;

  std::map<char, int> occurrenceMap;

  std::string::iterator iter;

  for (iter = input.begin (); iter != input.end (); iter++)
    {
      if (!isalpha ((char) (*iter)))
	continue;
      if (!islower ((char) (*iter)))
	continue;
      std::pair<std::map<char, int>::iterator, bool> ret;
      ret = occurrenceMap.insert (std::pair<char, int> (*iter, 1));
      if (ret.second == false) // already in array
	{
	  *iter = '?';
	  ret.first->second++;
	}
    }

  cout << "outputString=";
  map<const char, int>::iterator findMapValue;
  for (iter = input.begin (); iter != input.end (); iter++)
    {
      if (!isalpha ((char) (*iter)))
	continue;
      if (!islower ((char) (*iter)))
	continue;
      findMapValue = occurrenceMap.find ((char) (*iter));
      if (findMapValue != occurrenceMap.end ())
	{
	  int value = occurrenceMap[*iter];
	  if (value > 1)
	    {
	      cout << value;
	    }
	  cout << (char) (*iter);
	}
      else
	{
	  cout << "should not happen" << endl;
	}
    }
  cout << "\n" << endl;
  printf ("End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

}

