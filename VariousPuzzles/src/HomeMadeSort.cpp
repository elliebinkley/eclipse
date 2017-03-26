//============================================================================
// Name        : HomeMadeSort.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : a program that does three things
//               a.  does a sort of an array of integers three different ways
//                   i. using qsort
//                   ii. using a home-made slow way.
//                   iii.using stl:sort algorithm and stl:unique algorith
//                   iv. using stl:set
//               b.  Given an array of characters which form a sentence of words,
//                   write an efficient algorithm to
//                   reverse the order of the words (not characters) in it.
//                   See reverseWords.coo
//               c.  do the puzzle documented in reverseWords.cpp
//               d.  do a simple input/output puzzle from hackerRack in inputOutputExample.cpp
//               e.  do the puzzle in the Bloomberg string test. See BloombergStringTest.cpp
//============================================================================

#include <array>
#include <set>
#include <iostream>
#include <string>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "stdio.h"
#include "HomeMadeSort.hpp"
#include "Utilities.hpp"

using namespace std;

extern int cmp( const void *a, const void*b );
extern void print( const int array[], int size );
extern void mySortUsingQsort();
extern void mySortVerySlowly();
extern void mySortUsingSTL_SET();
extern void mySortUsingSTLSortAlgorithm();
extern int getLowest( int array[], int index, int size );

void homeMadeSort()
{
    T_START

    cout
            << "Sort an array of size n containing integers between 1 and K, given a temporary scratch integer array of size K."
            << endl;

    // Method 1, use qsort, and then eliminate duplicates.
    // initialize data
    cout << "Method 1: sort using qsort" << endl;
    mySortUsingQsort();
    cout << "End Method 1\n\n" << endl;

    // method 2
    cout
            << "Sort an array of size n containing integers between 1 and K, given a temporary scratch integer array of size K."
            << endl;
    cout << "Method 2: swapping entries" << endl;
    mySortVerySlowly();
    cout << "End Method 2\n\n" << endl;

    cout << "Method 3: Using std:map" << endl;
    cout
            << "Sort an array of size n containing integers between 1 and K, given a temporary scratch integer array of size K."
            << endl;
    mySortUsingSTL_SET();
    cout << "End Method 3\n\n" << endl;

    // method 4; Use STL sort algorithm
    cout << "Sort an array of names" << endl;
    cout << "Method 4: Using STL Sort" << endl;
    mySortUsingSTLSortAlgorithm();
    cout << "End Method 4\n\n" << endl;

    T_END
    return;
}

void mySortUsingQsort()
{
    T_START

    int n = 100;
    int k = 20;
    int myArray[n];
    for( int i = 0; i < n; i++ )
    {
        myArray[i] = rand() % k;
    }
    cout << "unsorted array=" << endl;
    print( myArray, n );

    // sort the array in-place.

    qsort( myArray, n, sizeof(int), cmp );
    cout << "sorted array=" << endl;
    print( myArray, n );

    // eliminate duplicates and put uniques in noDupArray[]
    int noDupArray[k];
    int k_index = 0;
    for( int i = 1; i < n; i++ )
    {
        if( myArray[i] != myArray[i - 1] )
        {
            if( k_index == k )
            {
                cout << "Bug:number of unique entries are > " << k << endl;
                continue;
            }
            noDupArray[k_index] = myArray[i];
            k_index++;
        }
    }
    if( k_index != k )
    {
        cout << "Number of unique entries are = " << k_index << endl;
    }

    cout << "\nsorted array(with no duplicates)=" << endl;
    print( noDupArray, k_index );

    T_END

}

// compare function; used by qsort.
// return > 0 if a>b, return <0 if b>a, if equal return 0.
int cmp( const void *a, const void *b )
{
    return (* (int*) a - * (int*) b);
}

// does the same thing but crudely, by iterating through the array (i) and getting the lowest value
// beyond i and  putting it at i. The value at i goes where the other value was swapped from.
// This has terrible performance!!!!
void mySortVerySlowly()
{
    T_START

    int n = 100;
    int k = 50;
    int array1[n], array2[k];

    // initialize the data.
    for( int i = 0; i < n; i++ )
    {
        array1[i] = rand() % k;
    }

    cout << "unsorted array=" << endl;
    print( array1, n );

    int k_index = 0;
    array2[k_index] = getLowest( array1, 0, n );
    int value = array2[k_index++];
    for( int x = 1; x < n; x++ )
    {
        value = getLowest( array1, x, n );
        if( value > array2[k_index - 1] )
        {
            array2[k_index] = value;
            k_index++;
        }
    }

    cout << "sorted array with no duplicates" << endl;
    print( array2, k_index );
    cout << " modified original array with duplicates " << endl;
    print( array1, n );

    T_END
}

// get the lowest value in the array, starting at index and put it at array[index] and return the value.
// size is the size of the array.
int getLowest( int array[], int index, int size )
{
    int lowestValue = array[index];
    int lowestValueIndex = index;
    for( int i = index + 1; i < size; i++ )
    {
        if( array[i] < lowestValue )
        {
            lowestValue = array[i];
            lowestValueIndex = i;
        }
    }
    int temp = array[index];
    array[index] = array[lowestValueIndex];
    array[lowestValueIndex] = temp;

    return lowestValue;
}

void print( const int array[], int size )
{
    for( int i = 0; i < size; i++ )
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

// use the std::sort to sort the array, then use std:unique to remove duplicates.
// sort an array of names contained in a class with first name and last name.
void mySortUsingSTLSortAlgorithm()
{
    T_START

    // initialize the array
    std::vector<NameList> vectorA;

    std::array<std::string, 5> firstNameArray { "james", "larry", "peter", "max", "jimmy" };
    std::array<std::string, 5> lastNameArray { "smith", "jones", "underwood", "clinton", "trump" };
    for( int i = 0; i < 5; i++ )
    {
        int k = rand() % 5;
        NameList a( firstNameArray[k], lastNameArray[k] );
        vectorA.push_back( a );
    }

    cout << "pre-sorted vector=" << endl;
    for( const auto &n : vectorA )
    {
        n.print();
    }
    cout << endl;

    // sort the vectorA
    std::sort( vectorA.begin(), vectorA.end() );

    // remove duplicates
    // using default comparison:
    std::vector<NameList>::iterator it;
    it = std::unique( vectorA.begin(), vectorA.end() );
    vectorA.resize( std::distance( vectorA.begin(), it ) );

    cout << "sorted vector(without duplicates)=" << endl;
    for( auto &n : vectorA )
    {
        n.print();
    }
    cout << endl;

    T_END

}

// use the std::set to sort the array and eliminate duplicates as well
void mySortUsingSTL_SET()
{
    T_START

    const int n = 100;
    const int k = 20;

    // initialize a data set
    std::array<int, n> myArray;
    std::array<int, n>::iterator myArrayIter;
    for( myArrayIter = myArray.begin(); myArrayIter != myArray.end(); myArrayIter++ )
    {
        int x = rand() % k;
        * myArrayIter = x;
    }
    // print initial data
    cout << "unsorted array=" << endl;
    for( auto &item : myArray )
    {
        cout << item << " ";
    }
    cout << endl;

    // put into a set; each element is unique. It will be sorted and duplicates eliminated.
    std::set<int> mySet( myArray.begin(), myArrayIter );

    cout << "sorted array with no duplicates=" << endl;
    for( auto elem : mySet )
    {
        cout << elem << " ";
    }
    cout << endl;

    T_END
}
