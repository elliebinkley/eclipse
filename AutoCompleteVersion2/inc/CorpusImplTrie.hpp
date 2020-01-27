/*
 * AutoCpmplete.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: laurence
 */
#ifndef INC_CORPUSIMPLTRIE_HPP_
#define INC_CORPUSIMPLTRIE_HPP_

#include <iostream>
#include <string>
#include "Corpus.hpp"
#include "TopLetter.hpp"

using namespace std;

class CorpusImplTrie : public Corpus
{
public:
   CorpusImplTrie() = default;                   // CTOR
   virtual ~CorpusImplTrie();                    // DTOR
   virtual void print( std::ostream& s ) const;  // Print the tree to stream s
   virtual void validate() const;

protected:
   virtual void insert( const std::string& phrase );
   virtual void searchImpl( const std::string& query, unsigned int maxResults );

private:
   TopLetter m_topLetter;   // Represents to trie structure for all phrases.
};

#endif /* INC_CORPUSIMPLTRIE_HPP_*/
