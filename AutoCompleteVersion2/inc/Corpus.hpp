/*
 * Corpus.hpp
 *
 *  Created on: Jan 5, 2020
 *      Author: laurence
 */

#ifndef CORPUS_HPP_
#define CORPUS_HPP_

#include <condition_variable>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


using namespace std;

class Corpus
{
public:
   Corpus() = default;           // CTOR
   virtual ~Corpus();;   // DTOR

   void cleanup();
   bool checkPhrase( std::string& phrase);
   bool checkInput( const std::string& phrase );
   inline size_t getCountChars() const { return m_countChars; };
   inline size_t getCountPhrases() const { return m_countPhrases; }
   inline size_t getMemorySize() const { return m_memorySizeRssAnon; };
   bool isNumber( const std::string& str );
   virtual bool initialize( char* fileName );
   virtual void print( std::ostream& s ) const = 0; // Print the tree.
   void search( const std::string& query, unsigned int maxResults );
   virtual void validate() const = 0;

protected:
   size_t calculateMemorySize();
   virtual void insert( const std::string& phrase ) = 0;
   int parseLine(char* line) const;
   virtual void searchImpl( const std::string& query, unsigned int maxResults ) = 0;

   size_t m_countPhrases = 0;
   size_t m_countChars = 0;
   size_t   m_memorySizeRssAnon = 0;    // memory size instrumentation for RSS anonymous.

private:
   void init();
   static bool runSearchThread( Corpus* corpus, const std::string& query, unsigned int maxResults );
   unsigned int m_maxThreads = 0;
   std::vector<std::future<bool>> m_currentFuture;
   std::mutex m_mtxSearch;
   std::condition_variable m_cv;
};

#endif  /*CORPUS_HPP_             */
