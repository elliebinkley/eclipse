/*
 * BoostLogWrapper.hpp
 *
 *  Created on: Nov 28, 2019
 *      Author: laurence
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <cstddef>
#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <fstream>


/* Singleton for configure Boost::Logging and verbosity.
 * Purpose: Expose the Boost flush method.
 * Usage: BoostLogWrapper::getInstance()->flushLogger();
 */
class Config
{
typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
typedef boost::shared_ptr< text_sink > MySink;

public:
   static Config* getInstance();
   inline void flushLogger() { m_sink->flush(); }

   inline bool getInteractive()  { return m_interactive; }
   inline bool getPrintResults() { return m_printResults; }
   inline bool getPrintCorpus() const { return m_printCorpus; }
   inline uint8_t getCorpusVersion() const { return m_CorpusVersion; }
   inline bool getVerbose()      { return m_verbose; }
   inline uint32_t getNumValidateErrors()   { return m_numValidateErrors; }
   inline void incrementValidateErrors() { m_numValidateErrors++; }
   inline void setCorpusVersion(uint8_t version ) { m_CorpusVersion = version; }
   inline void setInteractive( bool interactive) { m_interactive = interactive; }
   inline void setPrintCorpus(bool printCorpus) { m_printCorpus = printCorpus; }
   inline void setPrintResults( bool printResults) { m_printResults = printResults; }
   inline void setVerbose( bool verbose) { m_verbose = verbose; }


private:
   // set logging file name
   static constexpr std::string_view fileName = "autoComplete.log";

   Config();
   Config( Config& wrapper );  // no implementation

   static Config *m_instance;
   bool m_interactive;       // true to print to screen of user prompts.
   bool m_verbose;           // true to print the number of duplicates associated with
                             // each found search result.
   bool m_printResults;      // if false, prints just number of search results,
                             // but no actual results; set to false to get performance
                             // numbers without performance overhead of printing.
   bool m_printCorpus;       // if true, prints Corpus after initialization to cout;
                             // used for debugging Corpus.
   uint8_t m_CorpusVersion;  // chose which corpus version to use 0=Trie, 1=Map
   uint32_t m_numValidateErrors;  // number of Errors validate() caught.

   MySink m_sink;            // Boost logger sink.



};


#endif /* CONFIG_HPP_ */
