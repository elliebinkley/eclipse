//============================================================================
// Name        : DuplicateEntries.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : Given an array of size N in which every number is between 1 and N,
//             : write a simple program to determine if there are any duplicates in it.
//============================================================================

#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
#include <unordered_map>

using namespace std;

#define ARRAY_SIZE 50
static void initArray (std::array<int, ARRAY_SIZE> **arrayPtrPtr);
static std::array<int, ARRAY_SIZE> *stdArray = 0;

int
main ()
{
  cout << "Makefile Project Autotools chain" << endl;
  cout
      << "Given an array of size N in which every number is between 1 and N, write a simple "
      << "program to determine if there are any duplicates in it.. Print out the nnumber of duplicates detected"
	  << "2 or more of the same number is considered one duplicate." << endl;

  // method 1
  cout << "\nmethod 1 using C and no sorting N**2" << endl;

  // initialize the data
  int myarray[100];
  for( int i = 0; i < 100; i++ )
  {
      myarray[i] = rand ();
  }
  myarray[49] = myarray[99];  // insert a duplicate

  // find duplicates

  // for each value check if there is another identical value.
  for( int i = 0; i < 100; i++ )
  {
      int value = myarray[i];
      for( int j = 0; j < 100; j++ )
      {
	      if (i == j)
	      continue; // don't compare the same index.
	      if( value == myarray[j] )
	      {
	           cout << "duplicate value=" << value << "  index at " << i
		            << " and " << j << endl;
	      }
	   }
  }

  cout << "\nmethod 2 using C++ and STL and use sorting (2N) " << endl;
  //  set up array
  initArray( &stdArray );

  // find duplicates.
  // 1. sort the array using STL sort algorithm.
  std::sort( stdArray->begin(), stdArray->end() );

  // 2. look for adjacent duplicates. start at index 1
  for (unsigned int i = 1; i < stdArray->size (); i++)
  {
      if( (*stdArray)[i] == (*stdArray)[i - 1] )
	  {
	      cout << "item[" << i - 1 << "] and item[" << i << "] are = "
	           << (*stdArray)[i] << endl;
	}
  }

  cout << "\nmethod 3 using C++ and STL and map (1N)" << endl;
  //  set up array
  initArray( &stdArray );

  // use a ordered map to find duplicates.
  std::map<int, int> occurrenceMap { };
  int i = 0;
  for (const auto &s : *stdArray)  // similar to std::map<int,int>::iterator s;
  {
      /*
       * Can't do this since duplicates are overwritten; need to for check existence
       * occurrenceMap[s] = 1;
       */
      auto x = occurrenceMap.find (s);
      if( x == occurrenceMap.end() )
	  {
	      // not present; insert into map
	      /*  old stylem insert
	      std::pair<int,int> mypair(s,1);
	      std::pair<std::map<const int,int>::iterator, bool> insertion = occurrenceMap.insert(mypair); // occurrenceMap[s] = 1;
	      if( insertion.second == false)
	      {
	          // already present....
	          cout << "tried to insert <" << s << ",1>"
	               << " result=pair<" << insertion.first->first << "," << insertion.first->second
	               << "> bool=" << insertion.second << endl;
	      }
	      */
	      // use auto to insert
	      occurrenceMap[s] = 1;
	  }
      else
	  {
	      x->second++;    // increment occurrence count.
	      // cout << "duplicate detected for stdArray3["<< i << "]=" << s << " numDups=" << x->second << endl;
	  }
      i++;
    }
  // check results
  for( const auto &s : occurrenceMap )
  {
      if( s.second > 1 )
	  {
	  cout << "item=" << s.first << " has " << s.second << " occurrences"
	       << endl;
	  }
   }

   // method 4 : C++ and using hash tables; faster than a map ..
   cout << "\nmethod 4 using C++ and STL and hash table (1N)" << endl;
   //  set up array
   initArray( &stdArray );

   // find duplicates; unordered map uses hashing; faster than ordered amp (which uses binary search ); no duplicates allowed?
   std::unordered_map<int, int> map { };
   for( auto &s : *stdArray )
   {
       auto mapEntry = map.find (s);
       if( mapEntry == map.end() )
	   {
	       map[s] = 1;  //not present; insert
	   }
       else
	   {
	       mapEntry->second++;
	   }
   }
   // print results
   for (auto &x : map)
   {
       if( x.second > 1 )
	   {
	      cout << "item=" << x.first << " has " << x.second << " occurrences"
	           << endl;
	   }
   }

   if( stdArray ) delete stdArray;
   return 0;
}

static void
initArray( std::array<int, ARRAY_SIZE> **arrayPtrPtr )
{
   if( *arrayPtrPtr )  delete *arrayPtrPtr;
   std::array<int, ARRAY_SIZE> *array = new std::array<int, ARRAY_SIZE> { };
   *arrayPtrPtr = array;

   for( auto &s : *array )
   {
       s = rand ();
   }

   // make duplicates

   (*array)[4] = (*array)[8];
   (*array)[12] = (*array)[16];
   (*array)[20] = (*array)[16];

    return;
}
