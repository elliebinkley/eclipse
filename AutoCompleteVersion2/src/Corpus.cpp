/*
 * Corpus.cpp
 *
 *  Created on: Jan 5, 2020
 *      Author: laurence
 */
#include <chrono>
#include "../inc/BoostMacros.hpp"
#include "../inc/Corpus.hpp"
#include "../inc/Config.hpp"

Corpus::~Corpus()
{
   cleanup();
}

// wait for searches to finish
void Corpus::cleanup()
{
   for( auto iter = m_currentFuture.begin(); iter != m_currentFuture.end(); iter++ )
   {
      std::chrono::milliseconds span(0);
      iter->wait();
   }

   Config::getInstance()->flushLogger();
}

// Return amount of RSS anonymous memory this process uses in KB.
size_t Corpus::calculateMemorySize()
{
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while( fgets(line, 128, file) != NULL )
    {
        if( strncmp( line, "RssAnon:", 8 ) == 0)
        {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

// check for valid user command input
bool Corpus::checkInput( const std::string& input )
{
   std::stringstream ss;
   bool interactive = Config::getInstance()->getInteractive();
   for( const auto letter : input )
   {
      if( !( isalpha(letter) || isblank(letter) ))
      {
         ss << " Illegal Character:\"" << letter << "\" in line:\""
               << input << "\" ;";
         GEN_DBG1( ss.str() );
         if( interactive ) cout << ss.str() << endl;
         return false;
      }
   }
   return true;
}

// check for valid Corpus input, remove leading/trailing spaces, convert upper to lower
bool Corpus::checkPhrase( std::string& phrase)
{
   // remove leading blanks.
   while( isblank(phrase.front()) )
   {
      phrase.erase(phrase.begin() );
   }

   // remove trailing blanks.
   while ( isblank(phrase.back()))
   {
      phrase.erase(phrase.end() - 1 );
   }

   // check for valid characters and convert to lower case if not blank
   for( auto iter = phrase.begin(); iter != phrase.end(); iter++ )
   {
      bool isAlpha = isalpha(*iter);
      bool isBlank = isblank(*iter);

      if  ( !isAlpha && !isBlank )
      {
         stringstream ss;
         ss << " Illegal Character:\"" << *iter << "\" in phrase:\""
               << phrase << "\" ;";
         GEN_DBG1( ss.str() );
         std::cerr << "error:" << ss.str() << endl;
         return false;
      }

      if ( isAlpha && !islower(*iter))
      {
         *iter = tolower(*iter);
      }
   }

   if ( phrase.size() == 0 )
   {
      stringstream ss;
      ss << " null or all blank phrase; discarding ..." ;
      GEN_DBG1( ss.str() );
      std::cerr << "error:" << ss.str() << endl;
      return false;
   }
   return true;
}

// initalize thread count
void Corpus::init()
{
   m_maxThreads = std::thread::hardware_concurrency();
   if( Config::getInstance()->getInteractive() )
   {
      // only do one search at a time
      m_maxThreads = 1;
   }

   GEN_DBG2("m_maxThreads=", maxThreads );
}

bool Corpus::initialize( char* fileName )
{
   std::chrono::system_clock::time_point beforeTime =
                    std::chrono::system_clock::now();
   init();

   size_t beforeMem = calculateMemorySize();  // before

   // read input file and populate Corpus dictionary
   std::ifstream infile( fileName, std::ifstream::in);
   if( !infile.good() )
   {
      stringstream ss;
      ss << "bad input file " << fileName;
      GEN_DBG1( ss.str() );
      std::cerr << ss.str() << endl;
      return false;
   }

   std::string line;
   while( std::getline( infile, line ) )
   {
      if( checkPhrase( line ) )
      {
         insert( line );
         m_countPhrases++;
         m_countChars = m_countChars + line.size();
      }
   }

   if ( m_countPhrases == 0 )
   {
      std::cerr << "exiting!! No valid phrases found in file:" << fileName << std::endl;
      return false;
   }

   m_memorySizeRssAnon = calculateMemorySize() - beforeMem;

   if( Config::getInstance()->getVerbose())
   {
      std::chrono::system_clock::time_point afterTime =
                std::chrono::system_clock::now();

      uint64_t duration = (std::chrono::duration_cast<std::chrono::microseconds>)( afterTime - beforeTime ).count();
      uint32_t perPhrase = duration/getCountPhrases();

      cout << "corpus.initialization of " << getCountPhrases()
           << " phrases and " << getCountChars() << " characters took << " << duration << "us. e.i. "
           << perPhrase << "us./phrase" << std::endl;

   }
   return true;
}


bool Corpus::isNumber(const string& str)
{
   string::const_iterator itr = str.begin();
   while ( itr != str.end() && isdigit(*itr)) ++itr;
   return ( !str.empty() && itr == str.end() );
}

int Corpus::parseLine(char* line) const
{
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

bool Corpus::runSearchThread( Corpus* corpus, const std::string& query, unsigned int maxResults )
{
   std::chrono::system_clock::time_point before =  std::chrono::system_clock::now();

   corpus->searchImpl( query, maxResults );

   std::chrono::system_clock::time_point after =  std::chrono::system_clock::now();

   uint64_t duration = (std::chrono::duration_cast<
         std::chrono::microseconds>)( after - before ).count();
   if( Config::getInstance()->getInteractive() &&  Config::getInstance()->getVerbose() )
   {
      cout << "query:\"" << query << "\","
           <<  maxResults << " duration(us):" <<  duration << endl;
   }
   GEN_DBG6("corpus.search for query:\"",query,"\" maxResults:", maxResults," duration(us):", duration );

   // notify that the search is done.
   std::unique_lock<std::mutex> lck(corpus->m_mtxSearch);
   corpus->m_cv.notify_all();
   return true;
}

void Corpus::search( const std::string& query, unsigned int maxResults )
{
   std::unique_lock<std::mutex> lck(m_mtxSearch);

   // clean up old searches that are finished
   for( auto iter = m_currentFuture.begin(); iter != m_currentFuture.end(); )
   {
      std::chrono::milliseconds span(0);
      if ( iter->wait_for(span) == std::future_status::ready )
      {
         iter = m_currentFuture.erase(iter);    // search completed; delete ...
      }
      else
      {
         iter++;
      }
   }

   // wait if too many searches are still running
   if ( m_currentFuture.size() >= m_maxThreads )
   {
      cout << "waiting" << endl;
      m_cv.wait(lck); // wait until a search ends.
      cout << "waiting done" << endl;
   }

   // start new search
   std::future<bool> fut = std::async ( Corpus::runSearchThread, this, query, maxResults );
   m_currentFuture.push_back(std::move(fut) );

}


