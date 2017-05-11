/*
 * PThreadUtilities.hpp
 *
 *  Created on: May 10, 2017
 *      Author: USER
 */

#ifndef PTHREADUTILITIES_HPP_
#define PTHREADUTILITIES_HPP_

#include <atomic>


typedef volatile std::atomic<unsigned int> NUMERROR_TYPE;

void throwARunTimeError( const char* functionName, bool doThrow, NUMERROR_TYPE* const errorPtr, int line, int retVal = 0 );
void throwARunTimeError( const char* functionName, bool doThrow, NUMERROR_TYPE* const errorPtr,
                         const char* fileName, int line, int retVal = 0  );

#endif /* PTHREADUTILITIES_HPP_ */
