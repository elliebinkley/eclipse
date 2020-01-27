/*
 * Base.hpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#ifndef LETTER_HPP_
#define LETTER_HPP_

#include <ostream>
#include <memory>
#include <vector>
#include <bitset>
#include "BaseLetter.hpp"
#include "EopSuffix.hpp"
#include "EndOfPhrase.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <iostream>
#include "FoundItemsImplTrie.hpp"

// Represents a letter and any children letters.
// Subtending letters are contained in m_lettermap
// Letters in m_c must be a-z or space.
//
// m_lettermap rules:
// 1. 1-29 total entries; 26 lower case(a-z), space, Eop('.'), or EopSuffix(":")
// 2. zero or one Eop classes,
// 3. zero or one EopSuffix classes,
// 4. If an EopSuffix exists, then no letter classes(a-z) exist.
// 5. If one or more letters exist, then no EopSuffix exists.
// 6. TopLetters is a "?".
// 7. m_lettermap entries should be unique and sorted
// 8. checked by validate().

typedef std::vector<std::unique_ptr<BaseLetter>> BL_Container;

class Letter : public BaseLetter
{
public:

   static constexpr char CLASS_NAME[] = "Letter";
   static size_t m_totalLetters;

   // used for sorting; need to have all the derived types defined
   struct keyCompare {
      bool operator()( const std::unique_ptr<BaseLetter>& ptr1, const char& c )
      {
         return ( ptr1->getChar() < c );
      }
   };

   Letter( char c ) : BaseLetter( c )  {}
   virtual ~Letter() {}

   void addEopToResults( std::string& path, FoundItemsImplTrie& results );
   void addEopSuffixToResults( std::string& path, const std::string& query, FoundItemsImplTrie& results);
   virtual void collectPhrases( std::string& path, const std::string& query, FoundItemsImplTrie& results );
   virtual inline const char* getClassName() { return CLASS_NAME; }
   void insert( const std::string& phrase, uint16_t index=0, uint16_t count=1 );
   virtual void print( std::ostream &ios, bool printTree, uint8_t level ) const;
   virtual void validate() const;

protected:
   void incrementEop(uint16_t count = 1);
   inline bool hasEop() const { return  m_descriptor[0]; }        // true if m_lettermap has an EOP
   inline bool hasEopSuffix() const { return  m_descriptor[1]; }  // true if m_lettermap has an EOPsuffix
   inline void setEop( bool value ) { m_descriptor[0] = value; }  // true if m_lettermap has an EOP
   inline void setEopSuffix(bool value) { m_descriptor[1] = value; } // true if m_lettermap has an EOPsuffix
   void validateChildren() const;

   BL_Container m_lettermap;

private:

   Letter(); // no implementation
   std::bitset<2> m_descriptor;
};

#endif /* LETTER_HPP_ */
