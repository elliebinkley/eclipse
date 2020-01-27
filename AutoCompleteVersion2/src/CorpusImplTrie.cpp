#include "../inc/CorpusImplTrie.hpp"

#include "../inc/BoostMacros.hpp"
#include "../inc/Config.hpp"
#include "../inc/Letter.hpp"

#include <algorithm>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

CorpusImplTrie::~CorpusImplTrie()
{
   cleanup();
}

void CorpusImplTrie::insert(const string &phrase)
{
   m_topLetter.insert( phrase );
}

// print the entire Corpus
void CorpusImplTrie::print(std::ostream &s) const
{
   if ( Config::getInstance()->getVerbose() && Config::getInstance()->getInteractive() )
   {
      cout << "     \"someText :X\" indicates someText phrase occurred X times as a duplicate in the input." << endl;
      cout << "     \"someText #X\" indicates someText phrase occurred X times as a duplicate in the input." << endl;
      cout << "     \"#\" indicates that this phrase matches the first part of another substring" << endl;
      cout << "     \":\" indicates that this phrase is NOT a substring" << endl;
      cout <<" Corpus:" << endl;
   }
   m_topLetter.print( cout, true, 0 );
   Config::getInstance()->flushLogger();
}

void CorpusImplTrie::searchImpl(const std::string& query, unsigned int maxResults )
{
   GEN_DBG1( query );

   FoundItemsImplTrie foundItems( maxResults );
   std::string path("");
   m_topLetter.collectPhrases( path, query, foundItems );
   foundItems.print();

   return;
}

// run audit on Corpus to ensure correctness..
void CorpusImplTrie::validate() const
{
   this->m_topLetter.validate();
   uint32_t numErrors = Config::getInstance()->getNumValidateErrors();
   if ( numErrors )
   {
      std::cerr << "Corpus::validate() found " << numErrors << " errors; check log for details" << std::endl;
   }
   else
   {
      std::cout << "Corpus::validate() succeeded" << std::endl;
   }

   Config::getInstance()->flushLogger();
}

