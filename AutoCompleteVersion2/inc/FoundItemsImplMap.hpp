/*
 * FoundItem.hpp
 *
 *  Created on: Oct 20, 2019
 *      Author: laurence
 */

#ifndef INC_FOUNDITEMSIMPLMAP_HPP_
#define INC_FOUNDITEMSIMPLMAP_HPP_

#include <list>
#include <map>
#include <stdint.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "FoundItems.hpp"

typedef uint32_t sz_NumItems;
typedef uint32_t sz_NumStr;

typedef std::multimap<sz_NumItems, std::string_view> FoundItemsMapMMap;

class FoundItemsImplMap : FoundItems
{
public:
   FoundItemsImplMap(sz_NumItems count ): m_count(count), m_printed(0) {};
   void insert(const sz_NumItems count, const std::string_view& strView);
   void print() const;
private:
   sz_NumItems m_count;    // total max number of items allowed.
   // zero means allow all.
   FoundItemsMapMMap m_mmap;  // found items
   sz_NumItems m_printed;  // number printed when m_count == 0
   std::mutex m_found;       // mutex for printing results
};

#endif /* INC_FOUNDITEMSIMPLMAP_HPP_ */
