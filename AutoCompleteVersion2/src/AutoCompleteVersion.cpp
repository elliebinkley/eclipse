//============================================================================
// Name        : AutoCompleteVersion.cpp
// Author      : Laurence Burley
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <chrono>
#include <iostream>

#include <string_view>
#include <string>
#include <typeinfo>

#include "../inc/BoostMacros.hpp"
#include "../inc/Config.hpp"
#include "../inc/CorpusImplTrie.hpp"
#include "../inc/CorpusImplMap.hpp"
#include "../inc/CorpusImplMapCompress.hpp"

using namespace std;

/*
 * TODO:
 * 1. make a test suite with many strings , randomly and language generated.
 * 2. add metrics to measure performance and memory.
 * 3. add threading.
 */
int main(int argc, char **argv)
{

   try
   {
      bool interactive = true;  // set to true for user prompts;
                                // false for automated or production testing;
                                // does not affect performance testing
      bool verbose = true;     // set to true to get more output details for debugging use;
                                // false for automated testing/performance testing or
                                // production mode
      bool printResults = false; // set to false for performance testing,
                                // true otherwise
      bool printCorpus = false ; // set to true to debug Corpus
      uint8_t corpusVersion = 0;// 0:Trie Corpus   1:MapVersion   2:MapVersionCompressed


      std::unique_ptr<Corpus> corpus = nullptr;
      BOOST_ON;

      // initialize config parameters
      Config* config = Config::getInstance();
      config->setInteractive( interactive );
      config->setVerbose( verbose );
      config->setPrintResults( printResults );
      config->setPrintCorpus( printCorpus );
      config->setCorpusVersion(corpusVersion);

      if ( interactive )
      {
         std::cout << "interactive=" << interactive << std::endl;
         std::cout << "verbose=" << verbose << std::endl;
         std::cout << "printResults=" << printResults << std::endl;
         std::cout << "printCorpus=" << printCorpus << std::endl;
      }

      // check input parameters
      // argv[1] == filename
      if( argc != 2 )
      {
         std::stringstream ss;
         ss << "Usage:" << argv[0] << " filename";
         GEN_DBG1(ss.str());
         std::cout << "No file specified" << std::endl;
         std::cout << "Usage:" << argv[0] << " filename";
         return EINVAL;
      }
      GEN_DBG2( argv[0], argv[1] );
      GEN_DBG2( "interactive=", interactive );
      GEN_DBG2( "verbose=", verbose );
      GEN_DBG2( "printResults=", printResults );
      GEN_DBG2( "printCorpus=", printCorpus );


      switch(  config->getCorpusVersion() )
      {
      case 0:
         corpus = std::make_unique<CorpusImplTrie>();
         if ( interactive ) std::cout << "Corpus Version: \"trie\"" << std::endl;
         GEN_DBG1( "Corpus Version: \"trie\"");
         break;
      case 1:
         corpus = std::make_unique<CorpusImplMap>();
         if ( interactive ) std::cout << "Corpus Version: \"map\"" << std::endl;
         GEN_DBG1( "Corpus Version: \"map\"");
         break;
      case 2:
         corpus = std::make_unique<CorpusImplMapCompress>();
         if ( interactive ) std::cout << "Corpus Version: \"mapCompress\"" << std::endl;
         GEN_DBG1( "Corpus Version: \"mapCompress\"");
         break;
      default:
         cerr << "invalid Corpus=" << config->getCorpusVersion() << endl;
         GEN_DBG2( "invalid Corpus=",config->getCorpusVersion() );
         return 5;
      }

      if( !corpus->initialize( argv[1] ))
      {
         std::cerr << "exiting ... " << std::endl;
         return -1;
      }

      // validate the map
      if( config->getVerbose() && config->getInteractive() )  corpus->validate();
      // print the map
      if( config->getPrintCorpus() ) corpus->print( cout );

      // print memory sizes....; see proc(5)
      GEN_DBG3( "Corpus RssAnon=", corpus->getMemorySize(), " kB");
      if( config->getVerbose() && config->getInteractive() )
      {
         std::cout << "Corpus RssAnon=" << corpus->getMemorySize() << " kB" << std::endl;
      }
      GEN_DBG3 ( "Corpus RssAnon=", corpus->getMemorySize(), " kB" );

      Config::getInstance()->flushLogger();

      if( config->getInteractive() )
      {
         cout << "Enter a query of the form \n\n \"complete,XXX,Y\" \n\n"
               "where XXX is a search string containing characters \n"
               "[A-Z],[a-z],or space and Y is a number \n"
               "for the max number of search string results." << endl;
         cout << ">>> ";
      }

      // get query input
      std::string input;
      while( std::getline( std::cin, input ))
      {
         if( input == " " )
         {
            break;
         }

         // parse fields using istringstream ..
         std::string command, query, strNum;
         std::istringstream ssInput(input);

         // check 1st field to be "command"
         std::getline(ssInput, command, ',');
         if ( command != "complete" )
         {
            std::stringstream ss;
            ss << " 1st field: not \"complete\":" << input;
            GEN_DBG1( ss.str() );
            if( interactive )
            {
               cout << ss.str() << endl;
               cout << "... continuing" << endl;
               cout << ">>> ";
            }
            continue;
         }

         // check 2nd field to be legal command string
         std::getline( ssInput, query, ',' );
         if ( !corpus->checkInput( query ) )
         {
            std::stringstream ss;
            ss << " 2nd field has illegal letter:\"" <<  input << "\"";
            GEN_DBG1( ss.str() );
            if( interactive )
            {
               cout << ss.str() << endl;
               cout << "... continuing" << endl;
               cout << ">>> ";
            }
            continue;
         }

         // check 3rd field to be positive integer
         std::getline( ssInput, strNum, ',');
         if ( !corpus->isNumber( strNum ) )
         {
            std::stringstream ss;
            ss << "3rd field: not a digit:" <<  input << "\"";
            GEN_DBG1( ss.str() );
            if( interactive )
            {
               cout << ss.str() << endl;
               cout << "... continuing" << endl;
               cout << ">>> ";
            }
            continue;
         }
         // input was OK, log ....
         GEN_DBG2("query input:", input );
         GEN_DBG2("1st field:", command );
         GEN_DBG2("2nd field:", query );
         GEN_DBG2("3rd field:", strNum );
         Config::getInstance()->flushLogger();

         // search for results
         unsigned int count = std::stoul (strNum,NULL,0);

         corpus->search( query, count );

         Config::getInstance()->flushLogger();

         if( interactive ) cout << ">>>";
      }   // get next search string

      //corpus->cleanup();
      stringstream ss;
      ss << argv[0] << " " << argv[1] << " completed successfully";
      GEN_DBG1( ss.str() );
      Config::getInstance()->flushLogger();

      return 0;
   }
   catch( const std::string& e )
   {
      std::cout << "Exception caught:" << e << std::endl;
   }
   catch( const std::exception& e )
   {
      std::cout << e.what() << std::endl;
   }
   catch(...)
   {
      std::cout << "unknown exception" << std::endl;
   }
   return 1;
}

