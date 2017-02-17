/*
 * myLogger
 *
 *  Created on: Feb 6, 2016
 *      Author: burley
 *      Cheap logger so lines are not intermixed when output by various threads. A cheap console logger..
 */

#ifndef MYLOGGER_HPP_

#include <string>

using namespace std;

#define T_LOG(text)  MyLogger::log(__FILE__, __FUNCTION__, __LINE__, text);
#define T_START      MyLogger::logStart(__FILE__, __FUNCTION__, __LINE__ );
#define T_END        MyLogger::logEnd(__FILE__, __FUNCTION__, __LINE__ );

class MyLogger
{
public:
	// logs a string
	static void log( const char *file, const char* function, int line, const std::string& str);

	// logs the beginning of a method
	static void logStart(const char *file, const char* function, int line );

	// logs the end  of a method
	static void logEnd(const char *file, const char* function, int line );

};


#endif /* MYLOGGER_HPP_ */
