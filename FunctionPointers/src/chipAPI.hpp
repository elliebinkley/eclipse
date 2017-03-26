/*
 * chipAPI.hpp
 *
 *  Created on: Mar 22, 2017
 *      Author: USER
 */

#ifndef CHIPAPI_HPP_
#define CHIPAPI_HPP_

extern void initializeInterface();
#define F_ARRAY_ENTRIES 2
#define F_ARRAYPARAM_ENTRIES 2

extern void (*fArray[F_ARRAY_ENTRIES])();
extern void* (*fArrayParams[F_ARRAYPARAM_ENTRIES])(void*);

struct F1ParametersAPI
{
    int parameter1;
    int parameter2;
    int parameter3;
    int* retval;
};

#define SZ_P 3
struct F2ParametersAPI
{
    long parameter1;
    char parameter2[SZ_P+1];
    bool parameter3;
    int* retval;
};

#define VAL_X  50
#define VAL_Y  100
#define VAL_Z  100
#define RET_GOOD  9000
#define STR1 "abc"

#endif /* CHIPAPI_HPP_ */
