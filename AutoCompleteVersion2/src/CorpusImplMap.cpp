#include "../inc/BoostMacros.hpp"
#include "../inc/Config.hpp"
#include "../inc/Letter.hpp"

#include <algorithm>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string_view>
#include <utility>

#include "../inc/CorpusImplMap.hpp"
#include "../inc/FoundItemsImplMap.hpp"

using namespace std;

CorpusImplMap::~CorpusImplMap()
{
   cleanup();
}

void CorpusImplMap::insert(const string &phrase)
{
   std::pair<std::string,uint32_t> pair(phrase, 1);
   auto retPair = m_corpusMap.insert(pair);
   if ( !retPair.second )
   {
      // insertion failed. Probably a dup; increment the count
      retPair.first->second++;
   }
   return;
}

// print the entire Corpus
void CorpusImplMap::print(std::ostream &s) const
{
   for( auto& iter: m_corpusMap)
   {
       std::cout << iter.first << " :" << iter.second;
   }
   return;
}

void CorpusImplMap::searchImpl( const std::string& query, unsigned int maxResults )
{
   FoundItemsImplMap foundItems( maxResults );

   auto it = std::equal_range( m_corpusMap.begin(), m_corpusMap.end(),
                               query, CorpusImplMap::CompareIt() );
   for ( auto rngIt = it.first; rngIt != it.second; rngIt++ )
   {
      foundItems.insert( rngIt->second, rngIt->first );
   }
   foundItems.print();
   return;
}

// print statistics and do any validate needed.
void CorpusImplMap::validate() const
{
   uint32_t numErrors = Config::getInstance()->getNumValidateErrors();
   if ( numErrors > 0  )
   {
      std::cout << "validate() found " << numErrors
            << " errors; See log file for details" << std::endl;
      GEN_DBG2("validate totalErrorCount=", numErrors );
   }
}
