/*
 * BoostMacros.hpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#ifndef BOOSTMACROS_HPP_
#define BOOSTMACROS_HPP_


#include <boost/log/trivial.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <iostream>

//#define DEF_BOOST

#ifdef DEF_BOOST
#define GEN_DBG1(p1)  BOOST_LOG_TRIVIAL(trace) << __FILE__ << " " << __LINE__ \
                     << " Function:" << __FUNCTION__ << "() " \
                     << p1;
#define GEN_DBG2(p1, p2)  BOOST_LOG_TRIVIAL(trace) << __FILE__ << " " << __LINE__ \
                     << " Function:" << __FUNCTION__ << "() " \
                     << p1 << " " << p2;

#define GEN_DBG3(p1, p2, p3)  BOOST_LOG_TRIVIAL(trace) << __FILE__ << " " << __LINE__ \
                     << " Function:" << __FUNCTION__ << "() " \
                     << p1 << " " << p2 << " " << p3;
#define GEN_DBG4(p1, p2, p3, p4)  BOOST_LOG_TRIVIAL(trace) << __FILE__ << " " << __LINE__ \
                     << " Function:" << __FUNCTION__ << "() " \
                     << p1 << " " << p2 << " " << p3 << " " << p4;
#define GEN_DBG5(p1, p2, p3, p4, p5)  BOOST_LOG_TRIVIAL(trace) << __FILE__ << " " << __LINE__ \
                     << " Function:" << __FUNCTION__ << "() " \
                     << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5;
#define GEN_DBG6(p1, p2, p3, p4, p5, p6)  BOOST_LOG_TRIVIAL(trace) << __FILE__ << " " << __LINE__ \
                     << " Function:" << __FUNCTION__ << "() " \
                     << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " << p6;
#define BOOST_ON     std::cout << "Note:Boost Logging is active " << std::endl;
#else
#define GEN_DBG1(p1) ;
#define GEN_DBG2(p1,p2) ;
#define GEN_DBG3(p1,p2,p3) ;
#define GEN_DBG4(p1,p2,p3,p4) ;
#define GEN_DBG5(p1,p2,p3,p4,p5) ;
#define GEN_DBG6(p1,p2,p3,p4,p5,p6) ;
#define BOOST_ON std::cout << "Note:Boost Logging is inactive " << std::endl;
#endif




#endif /* BOOSTMACROS_HPP_ */
