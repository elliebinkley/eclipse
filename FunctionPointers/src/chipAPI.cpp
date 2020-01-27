/*
 * chipAPI.cpp
 *
 *  Created on: Mar 22, 2017
 *  Author: Burley
 */

#include <cstdio>
#include <cassert>
#include <string.h>
#include "chipAPI.hpp"

// initialize global arrays of function pointers
void (*fArray[ F_ARRAY_ENTRIES])();
void* (*fArrayParams[F_ARRAYPARAM_ENTRIES])(void*);

void function1API();
void function2API();
void* function1ApiParams(void* params);
void* function2ApiParams(void* params);

//initialize the arrays with the function pointers
void initializeInterface()
{
    fArray[0] = &function1API;
    fArray[1] = &function2API;
    fArrayParams[0] = &function1ApiParams;
    fArrayParams[1] = &function2ApiParams;
}

void function1API()
{
    printf("\t%s\n", __FUNCTION__);
    return;
}

void function2API()
{
    printf("\t%s\n", __FUNCTION__);
    return;
}

void* function1ApiParams(void* params)
{
    printf("\t%s\n", __FUNCTION__);
    F1ParametersAPI* apiParams = ( F1ParametersAPI*) params;
    assert( apiParams->parameter1 == VAL_X);
    assert( apiParams->parameter2 == VAL_Y);
    assert( apiParams->parameter3 == VAL_Z);
    assert(apiParams->retval);
    *apiParams->retval =  RET_GOOD;
    return(apiParams->retval );
}

void* function2ApiParams(void* params)
{
    printf("\t%s\n", __FUNCTION__);
    F2ParametersAPI* apiParams = ( F2ParametersAPI*) params;
    assert( apiParams->parameter1 == VAL_X );
    assert( !strncmp(apiParams->parameter2, STR1, SZ_P ) );
    assert( apiParams->parameter3);
    assert(apiParams->retval);
    *apiParams->retval =  RET_GOOD;
    return (void*) apiParams->retval;
}





