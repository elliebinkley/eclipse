/*
 * Utilities.hpp
 *
 *  Created on: Feb 22, 2017
 *      Author: USER
 */

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <iostream>
#define TRACE 0

#define T_START if(TRACE)  { std::cout << "Start**  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << std::endl; std::cout.flush();}
#define T_END   if(TRACE)  { std::cout << "End  **  File:" << __FILE__ << " Line:" << __LINE__ << " Function:" << __FUNCTION__ << std::endl; std::cout.flush();}


#endif /* UTILITIES_HPP_ */
