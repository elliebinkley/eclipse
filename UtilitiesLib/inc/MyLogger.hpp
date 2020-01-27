/*
 * MyLogger.hpp
 *
 *  Created on: Oct 10, 2019
 *      Author: laurence
 *      Logger that prevents intermixed lines when output by various threads.
 *      Console Logger only.
 */

#ifndef INC_MYLOGGER_HPP_
#define INC_MYLOGGER_HPP_

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

#endif /* INC_MYLOGGER_HPP_ */
