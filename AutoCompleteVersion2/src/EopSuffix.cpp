/*
 * EopSuffix.cpp
 *
 *  Created on: Nov 30, 2019
 *      Author: laurence
 */

#include <sstream>
#include <iostream>
#include <cassert>
#include "../inc/EopSuffix.hpp"
#include "../inc/BoostMacros.hpp"

size_t EopSuffix::m_totalEopSuffix = 0;

EopSuffix::EopSuffix( const std::string& phrase, uint16_t index, uint16_t count ) : EndOfPhrase( true, count )
{
   m_suffix = phrase.substr( index, phrase.size() );
}


void EopSuffix::validate() const
{
   assert( getChar() == EndOfPhrase::EOP_SUFFIX );
   assert( getCount() > 0 );

   // update number of total bytes used by this class
   m_totalEopSuffix++;
}

void EopSuffix::print(std::ostream& ios, bool printTree, uint8_t level) const
{
   for ( uint8_t indent=0; indent < level; indent++) ios << "  ";

   ios << "EopSuffix " << getChar() << " " << getCount() << " suffix=" << this->m_suffix << std::endl;
}



