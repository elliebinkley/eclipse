/*
 * AutoCpmplete.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: laurence
 */
#ifndef INC_CORPUSIMPLMAP_HPP_
#define INC_CORPUSIMPLMAP_HPP_

#include <iostream>
#include <string>
#include <string_view>
#include "../inc/Corpus.hpp"

using namespace std;

typedef std::map<std::string,uint32_t> CorpusMap;

class CorpusImplMap : public Corpus
{
public:
   // heterogeneous comparison of string to pair
   struct CompareIt
   {
      bool operator() ( const std::pair<std::string, uint32_t>& aPair, const std::string& query ) const
      {
         return ( aPair.first.compare(0, query.size(), query) < 0 );
      }
      bool operator() ( const std::string& query, const std::pair<std::string, uint32_t>& aPair) const
      {
         return( aPair.first.compare(0, query.size(), query) > 0 );
      }
   };

   CorpusImplMap() = default;;
   virtual ~CorpusImplMap();

   void compressCorpus();
   virtual void print( std::ostream& s ) const; // Print the tree.
   virtual void validate() const;

protected:
   virtual void insert( const std::string& phrase );
   virtual void searchImpl( const std::string& query, unsigned int maxResults );
   CorpusMap m_corpusMap;

};

#endif // INC_CORPUSIMPLMAP_HPP_
