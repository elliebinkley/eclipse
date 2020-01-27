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
#include "../inc/FoundItemsImplMap.hpp"


// Only keep track of minimal number of items to be printed.
// Discard extra items
// If no limit on printing items, then just print out the item immediately
void FoundItemsImplMap::insert(const sz_NumItems count, const std::string_view& foundItem )
{
   if( m_count == 0 )
   {
      if( Config::getInstance()->getPrintResults() )
      {
         // no print result maximum; just print out result
         if ( Config::getInstance()->getVerbose() )
         {
            std::cout << " " <<  foundItem << " :"
               << count << std::endl;
         }
         else
         {
            std::cout << " " <<  foundItem << std::endl;
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
      if ( it->first >= count )
      {
         GEN_DBG4( "map full of higher occurring phrases; discard phrase: ", phrase, " #",
                   numOccurrences );
         return;
      }
      else
      {
         GEN_DBG4( "deleting item: ", it->second, " #", it->first );
         m_mmap.erase( it );
      }
   }

   // insert item
   std::pair<uint32_t, std::string_view> pairInsert = { count, foundItem };
   m_mmap.insert( pairInsert );
   GEN_DBG4( "inserting phrase: ", foundItem, " #", count );

   assert ( m_mmap.size() <= m_count );

   return;
}

void FoundItemsImplMap::print() const
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
         GEN_DBG2( "#found in m_mmap: ", m_mmap.size() );
      }
      else
      {
         std::cout << "#printed: " << this->m_printed << std::endl;
         GEN_DBG2(  "#printed: ", m_printed );
      }
   }
}


