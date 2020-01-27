/*
 * Eop1.cpp
 *
 *  Created on: Nov 30, 2019
 *      Author: laurence
 */

#include <sstream>
#include <iostream>
#include <cassert>
#include "../inc/EndOfPhrase.hpp"
#include "../inc/BoostMacros.hpp"

size_t EndOfPhrase::m_totalEOPs = 0;

void EndOfPhrase::validate() const
{
   assert( this->getChar() == EndOfPhrase::EOP );
   assert( this->getCount() > 0 );
   m_totalEOPs++;
}

void EndOfPhrase::print(std::ostream& ios, bool printTree, uint8_t level) const
{
   for ( uint8_t indent=0; indent < level; indent++)  ios << "  ";

   ios << "EOP " << getChar() << " " << getCount() << std::endl;
}

