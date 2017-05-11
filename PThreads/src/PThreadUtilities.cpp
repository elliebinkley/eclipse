/*
 * PThreadUtilities.cpp
 *
 *  Created on: May 10, 2017
 *      Author: USER
 */


#include <sstream>
#include <cstring>
#include <atomic>
#include <cassert>
#include "SharedLibrary/inc/MyLogger.hpp"
#include "../inc/PThreadUtilities.hpp"

 using namespace std;

// error handler,
// functionName: function Name where error occurred.
// doThrow:  throw an exception
// errorPtr: increment an error counter
// line:   Function Line Number for tracing
// retVal: indicate if the error value is passed in ( non-zero ), or is contained in errno ( zero).

void throwARunTimeError( const char* functionName, bool doThrow, NUMERROR_TYPE* const errorPtr, int line, int retVal  )
{
    assert( functionName );
    assert( line );

    if( errorPtr ) (*errorPtr)++;

    stringstream ss;
    if( retVal )
    {
        ss << "\nLine#=" << line << " " << functionName << " failed; retVal=" << strerror( retVal );
    }
    else
    {
        ss << "\nLine#=" << line << " " << functionName << " failed; perror=" << strerror( errno );
    }

    if( doThrow )
    {
        throw(std::runtime_error( ss.str().c_str() ));
    } else
    {
        ss << "..continuing..";
    }
    T_LOG( ss.str().c_str() );
    return;
}

void throwARunTimeError( const char* functionName, bool doThrow, NUMERROR_TYPE* const errorPtr,
                         const char* fileName, int line, int retVal  )
{

    assert( functionName );
    assert( fileName );
    assert( line );

    if( errorPtr ) (*errorPtr)++;


    stringstream ss;
    if( retVal )
    {
        ss << "\nFile=" << fileName << " Line#=" << line << " " << functionName << " failed; retVal=" << strerror( retVal );
    }
    else
    {
        ss << "\nFile=" << fileName << " Line#=" << line << " " << functionName << " failed; perror=" << strerror( errno );
    }

    if( doThrow )
    {
        throw(std::runtime_error( ss.str().c_str() ));
    } else
    {
        ss << "..continuing..";
    }
    T_LOG( ss.str().c_str() );
    return;
}





