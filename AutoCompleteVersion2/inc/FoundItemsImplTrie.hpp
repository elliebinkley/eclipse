/*
 * FoundItem.hpp
 *
 *  Created on: Oct 20, 2019
 *      Author: laurence
 */

#ifndef INC_FOUNDITEMSIMPLTRIE_HPP_
#define INC_FOUNDITEMSIMPLTRIE_HPP_

#include <list>
#include <map>
#include <mutex>
#include <stdint.h>
#include <string>
#include "FoundItems.hpp"

typedef uint32_t sz_NumItems;
typedef uint32_t sz_NumStr;
typedef std::multimap<uint32_t, std::string> FoundItemsTrieMMap;

class FoundItemsImplTrie : FoundItems
{
public:
   FoundItemsImplTrie(sz_NumItems count ): m_count(count), m_printed(0) {};
   void insert(const std::string& phrase, sz_NumStr numOccurrences );
   void print() const;
   static std::mutex m_mtxFnd;      // mutex for printing results
private:
   sz_NumItems m_count;             // total max number of items allowed.
   FoundItemsTrieMMap m_mmap;
   sz_NumItems m_printed;           // number printed when m_count == 0

};

#endif /* INC_FOUNDITEMSIMPLTRIE_HPP_ */
