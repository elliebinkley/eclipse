/*
 * TopLetter.cpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#include <sstream>
#include "../inc/BoostMacros.hpp"
#include "../inc/TopLetter.hpp"
#include "../inc/Config.hpp"

void TopLetter::validate() const
{
   bool isError = false;
   std::stringstream ss;

   // Initialize counts;
   Letter::m_totalLetters = 0;
   EndOfPhrase::m_totalEOPs = 0;
   EopSuffix::m_totalEopSuffix = 0;

   GEN_DBG2( "Letter size=", sizeof(Letter) );
   GEN_DBG2( "EndOfPhrase size=", sizeof(EndOfPhrase) );
   GEN_DBG2( "EopSuffix size=", sizeof(EopSuffix) );
   GEN_DBG2( "std::string size=", sizeof(std::string) );
   GEN_DBG2( "BL_Container=", sizeof(BL_Container) );
   GEN_DBG2( "std::bitset<2>=", sizeof(std::bitset<2>) );

   if ( hasEop() )
   {
      // topletter cannot be an eop.
      isError = true;
      ss << "error:Eop set for TopLetter=" << getChar ();
      Config::getInstance()->incrementValidateErrors();
   }

   if( getChar() != TopLetter::TOP_LETTER )
   {
      isError = true;
      ss << "error:invalidTopLetter=" << getChar ();
      Config::getInstance()->incrementValidateErrors();
   }

   if ( isError )
   {
      print(std::cout, false, 0);
      print(ss, false, 0);
      GEN_DBG1( ss.str() );
   }

   m_totalLetters++;
   validateChildren();

   if( Config::getInstance()->getInteractive() )
   {
      uint32_t numErrors = Config::getInstance()->getNumValidateErrors();
      if ( numErrors > 0  )
      {
         std::cout << "validate() found " << numErrors
               << " errors; See log file for details" << std::endl;
         GEN_DBG2("validate totalErrorCount=", numErrors );
      }

      // print Letter statistics
      std::stringstream ssStat("Corpus Statistics:");
      ssStat << " #Letters=" << Letter::m_totalLetters;
      ssStat << " #Eop=" << EndOfPhrase::m_totalEOPs;
      ssStat << " #EopSuffix=" << EopSuffix::m_totalEopSuffix;
      GEN_DBG1( ssStat.str() );
      std::cout << ssStat.str() << std::endl;

   }
}


