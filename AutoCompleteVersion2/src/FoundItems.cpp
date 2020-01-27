/*
 * FoundItems.cpp
 *
 *  Created on: Jan 16, 2020
 *      Author: laurence
 */

#include <mutex>
#include "../inc/FoundItems.hpp"

std::mutex FoundItems::m_mtxFnd;      // mutex for printing results


