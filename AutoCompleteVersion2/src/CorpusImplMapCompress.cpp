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

#include "../inc/CorpusImplMapCompress.hpp"
#include "../inc/FoundItemsImplMap.hpp"

using namespace std;

// reduce memory usage by moving the data from
// CorpusMap m_corpusMap to std::string
// and CorpusMapCompressed
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

CorpusImplMapCompress::~CorpusImplMapCompress()
{
   cleanup();
   if (m_corpusString) delete m_corpusString;
}

// reduce memory usage by moving the data from
// m_corpusMap to m_corpusString.
// Make a new map m_CorpusMapCompressed of string_views
// into m_corpusString.
void CorpusImplMapCompress::compressCorpus()
{
   size_t oldIndex = 0;
   size_t newIndex = 0;
   size_t countChars = 0;
   size_t countPhrases = 0;

   for( auto& iter : m_corpusMap )
   {
      countChars = countChars + iter.first.size();
      countPhrases++;
   }

   size_t maxChar = countChars + countPhrases + 100;
   if ( Config::getInstance()->getInteractive()  && Config::getInstance()->getVerbose())
   {
      cout << "countChars=" << countChars << " countPhrases=" << countPhrases
            << " maxChar=" << maxChar << std::endl;
   }
   GEN_DBG4( "countChars=", countChars, " countPhrases=", countPhrases);
   m_corpusString = (char*) new char[maxChar];

   // Compress by using string_view instead of std:: string
   // A string_view is less space than a string....
   // Take each string out of m_corpusMap, move it into m_corpusString
   // and insert a string_view of the string into m_corpusMapCompress.
   for( auto& iter:this->m_corpusMap )
   {
      assert(oldIndex < maxChar);
      assert(newIndex < maxChar);

      // copy each char from the phrase the char array
      for( auto& itStr: iter.first )
      {
         m_corpusString[newIndex] = itStr;
         newIndex++;
      }
      // make a string_view of the phrase
      std::string_view strView(&m_corpusString[oldIndex], iter.first.size() );

      // add a delimiter to separate phrases
      m_corpusString[newIndex++] = CorpusImplMapCompress::m_delim;
      oldIndex = newIndex;

      // insert pair<string_view,count> into the map
      auto iterCompress = m_corpusMapCompress.insert(std::make_pair(strView,iter.second ));
      assert( iterCompress.second );  // check for success
   }
}

bool CorpusImplMapCompress::initialize( char* fileName )
{
   bool ret=Corpus::initialize(fileName);

   // get sizes before and after compression
   size_t before = calculateMemorySize();
   if ( ret )
   {
      compressCorpus();
      m_corpusMap.clear();
   }
   // before memory not always freed, so subtract it out.
   m_memorySizeRssAnon = calculateMemorySize() - before;
   return ret;
}

// print the entire Corpus
void CorpusImplMapCompress::print(std::ostream &s) const
{
   for( auto& iter: m_corpusMapCompress)
   {
       std::cout << iter.first << " :" << iter.second << std::endl;
   }
   cout << "m_corpusString=" << m_corpusString << endl;
   return;
}

void CorpusImplMapCompress::searchImpl(const std::string& query, unsigned int maxResults )
{
   FoundItemsImplMap foundItems( maxResults );

   auto it = std::equal_range( m_corpusMapCompress.begin(), m_corpusMapCompress.end(),
                               query,  CorpusImplMapCompress::CompareItCompress() );
   for ( auto rngIt=it.first; rngIt != it.second; rngIt++ )
   {
      foundItems.insert(rngIt->second, rngIt->first);
   }
   foundItems.print();
   return;
}

