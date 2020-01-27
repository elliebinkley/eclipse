/*
 * Config.cpp
 *  Configuration parameters for
 *  1) boost logging ( where to log and log level )
 *  2) interactive  ( ask user questions)
 *  3) verbose      ( lots of output)
 *  4) validation errors ( runs audit on corpus)
 *  5  printResults ( prints output of search, turn off for performance testing )
 *  Created on: Nov 28, 2019
 *      Author: laurence
 */

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "../inc/Config.hpp"

Config* Config::m_instance = 0;

Config::Config():
      m_interactive(false),
      m_verbose(false),
      m_printResults( false),
      m_printCorpus(false),
      m_CorpusVersion(0),
      m_numValidateErrors(0),
      m_sink(boost::make_shared< text_sink >())
{
   // log to Config::filename
   m_sink->locked_backend()->
         add_stream( boost::make_shared< std::ofstream >( Config::fileName.data() ));
   boost::log::core::get()->add_sink( m_sink );

   // set filter level
   boost::log::core::get()->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::trace );
}

Config* Config::getInstance()
{
   if ( m_instance == NULL )
   {
      m_instance = new Config();
   }
   return m_instance;
}


