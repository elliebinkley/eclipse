/*
 * FoundItems.cpp
 *
 *  Created on: Oct 20, 2019
 *      Author: laurence
 */

#include "../inc/Config.hpp"
#include "../inc/BoostMacros.hpp"


#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <system_error>
#include "stdio.h"
#include "../inc/FoundItemsImplTrie.hpp"



// Only keep track of minimal number of items to be printed.
// Discard extra items
// If no limit on printing items, then just print out the item immediately
void FoundItemsImplTrie::insert(const std::string& phrase,  sz_NumStr numOccurrences )
{
   if( m_count == 0 )
   {
      if( Config::getInstance()->getPrintResults() )
      {
         // no print result maximum; just print out result
         if ( Config::getInstance()->getVerbose() )
         {
            std::cout << " " <<  phrase << " :"
               << numOccurrences << std::endl;
         }
         else
         {
            std::cout << " " <<  phrase << std::endl;
          }
       }
       this->m_printed++;
       return;
   }

   // if max number of items already exists see if new phrase should be discarded or
   // replace another phrase
   auto it = m_mmap.begin();     // sorted map; first item has the lowest number of duplicates
   if ( m_mmap.size() == m_count )
   {
      if ( it->first >= numOccurrences )
      {
         GEN_DBG4( "map size full; discard phrase: ", phrase, " #", numOccurrences );
         return;
      }
      else
      {
         GEN_DBG4( "deleting item: ", it->second, " #", it->first );
         m_mmap.erase( it );
      }
   }

   // insert item
   std::pair<uint32_t, std::string> pair = { numOccurrences, phrase };
   m_mmap.insert( pair );
   GEN_DBG4( "inserting phrase: ", phrase, " #", numOccurrences );

   assert ( m_mmap.size() <= m_count );

   return;
}

void FoundItemsImplTrie::print() const
{
   bool verbose = Config::getInstance()->getVerbose();
   bool printResults = Config::getInstance()->getPrintResults();

   std::unique_lock<std::mutex> lck(FoundItems::m_mtxFnd);

   if( printResults )
   {
      for (auto iter = m_mmap.rbegin(); iter != m_mmap.rend(); ++iter)
      {
         if ( verbose )
         {
            std::cout << " " <<  iter->second << " :" << iter->first << std::endl;
         }
         else
         {
            std::cout << " " <<  iter->second << std::endl;
         }
      }
   }

   if ( verbose )
   {
      if( m_count > 0 )
      {
         std::cout << "#found in m_mmap: " << m_mmap.size() << std::endl;
      }
      else
      {
         std::cout << "#printed: " << this->m_printed << std::endl;
      }
   }
}


