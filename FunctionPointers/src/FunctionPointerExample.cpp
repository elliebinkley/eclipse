//============================================================================
// Name        : OracleExamples.cpp
// Author      : larry Burley
// Version     :
// Copyright   :
// Description : Function Pointer Exercise.
//             : I did this because I couldn't remember function pointer syntax during an interview.
//             : See http://stackoverflow.com/questions/5488608/how-define-an-array-of-function-pointers-in-c
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include "chipAPI.hpp"

using namespace std;

struct ReturnValuesForFunction3
{
    int retval1;
    int retval2;
};

struct Func3Parameters
{
    int parameter1;
    int parameter2;
    int parameter3;
    ReturnValuesForFunction3  *results;
};

void function1();
void function2();
void* function3(void*);
void invokeSlot0API();
void invokeSlot1API();

#define TEST_START(x) cout << "Start: Test" << x << endl;
#define TEST_END(x) cout << "End: Test" << x++ << endl;

// define constants for testing purposes
#define P1  10
#define P2  20
#define P3  30
#define R1  1000
#define R2  2000

#define CHECK_INIT_F_ARRAY       {for( int i = 0; i < F_ARRAY_ENTRIES; i++ ) { assert(fArray[i]); }};
#define CHECK_INIT_F_ARRAYPARAMS {for( int i = 0; i < F_ARRAYPARAM_ENTRIES; i++ ) { assert(fArrayParams[i]); }};

int main() {
	cout << "Start:Running some examples on function pointers" << endl;
	int testId = 1;

	// declare, define and call a function pointer using the stack;
	TEST_START(testId)
	void (*functptr1)();
	functptr1 = &function1;
	functptr1();
	TEST_END(testId)

	// declare and define in one statement, and call a function pointer using the stack; better syntax
	TEST_START(testId)
	void (*functptr2)() = function2;  //  The "&: from previous example is optional .. both work.
	functptr2();
	TEST_END(testId)

	// function pointer that has return values and parameters
	TEST_START(testId)
	void* (*functptr3)(void*) = function3;
	// fill out parameters
	Func3Parameters* params = (Func3Parameters*) malloc( sizeof (Func3Parameters));
	params->parameter1 = P1;
	params->parameter2 = P2;
	params->parameter3 = P3;
	ReturnValuesForFunction3  *results = (ReturnValuesForFunction3*) malloc( sizeof (ReturnValuesForFunction3));
	params->results = results;
	// call function3()
    void* ptr3  = functptr3((void *)params);
    // check results;
	assert(ptr3 == results);
	assert(results->retval1 == R1);
	assert(results->retval2 == R2);
	// cleanup
	free( results );
	free( params );
	TEST_END(testId)

	// separate the API into a different files and use global memory to store the function pointer.
	// There are 2 arrays of functions pointers; one array with no parameters and one with parameters.
	TEST_START(testId)
    initializeInterface();
	CHECK_INIT_F_ARRAY
    // call the functions that don't require parameters.
    for( int i = 0; i < F_ARRAY_ENTRIES; i++)
    {
        fArray[i]();
    }
    CHECK_INIT_F_ARRAYPARAMS
    // call the functions that require parameters.
    invokeSlot0API();
    invokeSlot1API();

	TEST_END(testId)

	cout << "End:Running some Oracle examples" << endl;
	return 0;
}

void function1(void)
{
    printf("\t%s\n", __FUNCTION__);
       return;
}

void function2(void)
{
    printf("\t%s\n", __FUNCTION__);
       return;
}

void* function3(void *ptr)
{
    printf("\t%s\n", __FUNCTION__);
    assert(ptr);
    Func3Parameters* params =  ( Func3Parameters* ) ptr;
    assert(params->parameter1 == P1 );
    assert(params->parameter2 == P2 );
    assert(params->parameter3 == P3 );
    params->results->retval1 = R1;
    params->results->retval2 = R2;
    return (void*) (params->results);
}

// invoke the API;
void invokeSlot0API()
{
    F1ParametersAPI* params = (F1ParametersAPI*) malloc(sizeof(F1ParametersAPI));
    params->parameter1 = VAL_X;
    params->parameter2 = VAL_Y;
    params->parameter3 = VAL_Z;
    int* retval = (int*) malloc(sizeof(int));
    *retval = 0;
    params->retval = retval;
    void* ret = fArrayParams[0](params);
    assert( ret == retval);
    assert( *retval == RET_GOOD );
    free( params );
    free( retval );
}

void invokeSlot1API()
{

    F2ParametersAPI* params = (F2ParametersAPI*) malloc(sizeof(F2ParametersAPI));
    params->parameter1 = VAL_X;
    strncpy(params->parameter2, STR1, SZ_P+1);
    params->parameter3 = true;
    int* retval = (int*) malloc(sizeof(int));
    *retval = 0;
    params->retval = retval;
    void* ret = fArrayParams[1](params);
    assert( ret == retval);
    assert( *retval == RET_GOOD );
    free( params );
    free( retval );
}



