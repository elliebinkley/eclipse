//============================================================================
// Name        : HomeMadeSort.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : a program that does three things
//               a.  does a sort of an array of integers three different ways
//                   i. using qsort
//                   ii. using a home-made slow way.
//                   iii. using STL
//                   iv.  todo: using stl:sort algorithm
//               b.  Given an array of characters which form a sentence of words,
//                   write an efficient algorithm to
//                   reverse the order of the words (not characters) in it.
//                   See reverseWords.coo
//               c.  do the puzzle documented in reverseWords.cpp
//               d.  do a simple input/output puzzle from hackerRack in inputOutputExample.cpp
//               e.  do the puzzlein the Bloomberg string test.
//============================================================================

#include <array>
#include <iostream>
#include <string>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "stdio.h"

using namespace std;

extern int cmp(const void *a, const void*b);
extern void print(const int array[], int size);
extern void doIt();
extern void doItWithSTL();
extern int getLowest(int array[], int index, int size);
extern void reverseWords();
extern void createStairCase();
extern void inputOutputExample();
extern void bloombergStringTestVersion1();
extern void bloombergStringTestVersion2();

int main()
{
	printf("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	cout
			<< "Sort an array of size n containing integers between 1 and K, given a temporary scratch integer array of size K."
			<< endl;
	int size = 100;
	int myArray[size];

	// Method 1, use qsort..
	// populate the array
	for (int i = 0; i < size; i++) {
		myArray[i] = rand() & size;
	}
//    print(myArray, size);
	qsort(myArray, size, sizeof(int), cmp);
	//   print(myArray, size);
	printf("End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	// method 2
	doIt();
	// Method 3
	doItWithSTL();

	cout << "Done:  Sort an array of size n containing integers between 1 and K, given a temporary scratch integer array of size K.\n\n"
			<< endl;

	reverseWords();

	createStairCase();

	inputOutputExample();

	bloombergStringTestVersion1();
	bloombergStringTestVersion2();

	return 0;
}

// compare function; used by qsort.
// return > 0 if a>b, return <0 if b>a, if equal return 0.
int cmp(const void *a, const void *b)
{
	return (*(int*) a - *(int*) b);
}

// does the same thing but crudely, by iterating through the array (i) and getting the lowest value
// beyond i and  putting it at i. The value at i goes where the other value was swapped from.
void doIt()
{
	int k = 100;
	int array1[k], array2[k];

	for (int i = 0; i < k; i++)
	{
		array1[i] = rand() % k;
	}
	print(array1, k);

	for (int i = 0; i < k; i++) {
		array2[i] = getLowest(array1, i, k);
	}
	print(array2, k);
	print(array1, k);
}

// get the lowest value in the array, starting at index and put it at array[index] and return the value.
// size is the size of the array.
int getLowest(int array[], int index, int size)
{
	int lowestValue = array[index];
	int lowestValueIndex = index;
	for (int i = index + 1; i < size; i++)
	{
		if (array[i] < lowestValue)
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

void print(const int array[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << "item[" << i << "]=" << array[i] << endl;
	}
}

void doItWithSTL()
{
	printf("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	int size = 100;

	std::array<int, 100> array1;
	std::array<int, 100>::iterator array1Iter;
	std::array<int, 100>::iterator tempIter;

	// initialize the array.
	for (array1Iter = array1.begin(); array1Iter != array1.end();
			array1Iter++)
	{
		*array1Iter = rand() % size;
	}
	for (int i = 0; i < size; i++)
	{
		cout << "array1[" << i << "]= << " << (int) array1[i] << endl;
	}
	for (array1Iter = array1.begin(); array1Iter != array1.end();
			array1Iter++)
	{
		tempIter = array1Iter;
		tempIter++;
		for (; tempIter != array1.end(); tempIter++)
		{
			if (*array1Iter > *tempIter) {
				int x = *array1Iter;
				*array1Iter = *tempIter;
				*tempIter = x;
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		cout << "array1[" << i << "]= << " << (int) array1[i] << endl;
	}
	printf("End: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

