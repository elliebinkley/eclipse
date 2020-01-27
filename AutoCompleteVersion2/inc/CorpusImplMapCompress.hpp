/*
 * CorpusImplMapCompress.hpp
 *
 *  Created on: Jan 9, 2020
 *      Author: laurence
 */

#ifndef CORPUSIMPLMAPCOMPRESS_HPP_
#define CORPUSIMPLMAPCOMPRESS_HPP_


#include <iostream>
#include <string>
#include <string_view>
#include "../inc/CorpusImplMap.hpp"

using namespace std;

typedef std::map<std::string_view,uint32_t> CorpusMapCompressed;

class CorpusImplMapCompress : public CorpusImplMap
{
public:

   // heterogeneous comparison of string to pair
   struct CompareItCompress
   {
      bool operator() ( const std::pair<std::string_view, uint32_t>& aPair,
                        const std::string& query ) const
      {
         return ( aPair.first.compare(0, query.size(), query) < 0 );
      }
      bool operator() ( const std::string& query,
                        const std::pair<std::string_view, uint32_t>& aPair) const
      {
         return( aPair.first.compare(0, query.size(), query) > 0 );
      }
   };

   CorpusImplMapCompress(): m_corpusString(0) {}; // CTOR
   virtual ~CorpusImplMapCompress();              // DTOR

   void compressCorpus();
   virtual bool initialize( char* fileName );
   virtual void print( std::ostream& s ) const; // Print the tree.

protected:
   virtual void searchImpl( const std::string& query, unsigned int maxResults );

private:
   CorpusMapCompressed m_corpusMapCompress; // Map using string_views into corpusString;
   char* m_corpusString;
   static const char m_delim = ';';
};



#endif /* CORPUSIMPLMAPCOMPRESS_HPP_ */
