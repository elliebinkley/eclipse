/*
 * createStairCase.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: USER
 */
#include "stdio.h"
#include "string.h"

// given a number N; create a staircase pattern e.g.
//      x
//     xx
//    xxx
//   xxxx
//  xxxxx
// xxxxxx
// in this case N is 6.
void
createStairCase ()
{
  printf ("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
  int N = 6;
  char temp[N + 1];
  memset (temp, 0, N + 1);
  strncpy (temp, "ttttttttt", N);
  printf ("%s\n", temp);
  // for each of N lines ...
  for (int i = 1; i < N + 1; i++)
    {
      for (int j = 0; j < N; j++)
	{
	  temp[j] = 'x';
	}
      for (int k = 0; k < N - i; k++)
	{
	  temp[k] = ' ';
	}
      printf ("%s\n", temp);
    }
  printf ("End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

