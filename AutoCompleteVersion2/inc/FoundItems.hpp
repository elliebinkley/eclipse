/*
 * FoundItems.hpp
 *
 *  Created on: Jan 16, 2020
 *      Author: laurence
 */

#ifndef FOUNDITEMS_HPP_
#define FOUNDITEMS_HPP_

#include <mutex>

class FoundItems
{
public:
   static std::mutex m_mtxFnd;
};


#endif /* FOUNDITEMS_HPP_ */
