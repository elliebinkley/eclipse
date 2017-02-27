/*
 * DuplicateEntries.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: larry Burley
 *
 *
 *
 *
 *
 */

#include <iostream>
#include <iomanip>
#include <array>
#include <map>
#include <unordered_map>
#include "Utilities.hpp"

#define ARRAY_SIZE 100
#define EXPECTED_DUPS 4
extern void method1();
extern void method2();
extern void init( std::array<int, ARRAY_SIZE>& array );
extern void print( std::array<int, ARRAY_SIZE>& array );

using namespace std;

void duplicateEntries()
{
    std::cout
            << "Given an array of size N in which every number is between 1 and N, write a simple "
            << "program to determine if there are any duplicates in it.. Print out the number of duplicates detected"
            << "2 or more of the same number is considered one duplicate." << std::endl;
    method1();
    method2();
}

void
method1()
{
    // Method 1
    std::cout << "Method 1; using map" << std::endl;
    std::array<int, ARRAY_SIZE> testArray;
    init( testArray );
    print( testArray );

    int expectedDups = EXPECTED_DUPS;

    // use a ordered map to find duplicates.
    int numDups = 0;
    std::map<int, int> occurrenceMap;
    for( const auto &s : testArray )  // similar to std::map<int,int>::iterator s;
    {
        // check if it is in the map.. Note that insert of a dup will overwrite.
        auto x = occurrenceMap.find( s );
        if( x == occurrenceMap.end() )
        {
            occurrenceMap[s] = 1;
        }
        else // found
        {
            x->second++;    // increment occurrence count.
            if( x->second == 2 ) numDups++;
        }
    }
    std::cout << "duplicate count=" << numDups << std::endl;
    if( numDups != expectedDups )
    {
        std::cout << "FAIL; expected=" << expectedDups << "does not match found dups=" << numDups
                << std::endl;
    }
    std::cout << "Method 1 End" << std::endl;
    return;
}

void
method2()
{
    std::cout << "Method 2; using unordered map" << std::endl;
    // Method 1
    std::array<int, ARRAY_SIZE> testArray;
    init( testArray );

    int expectedDups = EXPECTED_DUPS;

    // use an unordered map to find duplicates. Hashes are faster than sorting in a map..
    int numDups = 0;
    std::unordered_map<int, int> occurrenceMap;
    for( const auto &s : testArray )  // similar to std::map<int,int>::iterator s;
    {
        // check if it is in the map.. Note that insert of a dup will overwrite.
        auto x = occurrenceMap.find( s );
        if( x == occurrenceMap.end() )
        {
            occurrenceMap[s] = 1;
        }
        else // found
        {
            x->second++;    // increment occurrence count.
            if( x->second == 2 ) numDups++;
        }
    }
    std::cout << "duplicate count=" << numDups << std::endl;
    if( numDups != expectedDups )
    {
        std::cout << "FAIL; expected=" << expectedDups << "does not match found dups=" << numDups
                << std::endl;
    }
    std::cout << "Method 2 End" << std::endl;
    return;
}

void
init( std::array<int, ARRAY_SIZE>& testArray )
{
    int range = 100;
    for( int i = 0; i < ARRAY_SIZE; i++ )
    {
        // produce a random number between 1-100 + ( i * 100)
        testArray[i] = (rand() % range) + (i * range);
    }
    // put in 4 dups;
    testArray[5] = testArray[50];
    testArray[80] = testArray[50];  // second dup
    testArray[99] = testArray[0];
    testArray[50] = testArray[51];
    testArray[30] = testArray[71];
    return;

}

void
print( std::array<int, ARRAY_SIZE>& array )
{
    int i = 0;
    std::cout << " input=" << std::endl;
    for( auto &x : array )
    {
        std::cout << setw(4) << x << " ";
        i++;
        if( (i % 25) == 0 )
        {
            std::cout << std::endl;
        }
    }
}

