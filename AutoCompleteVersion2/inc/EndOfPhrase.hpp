/*
 * EndOfPhrase.hpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#ifndef ENDOFPHRASE_HPP_
#define ENDOFPHRASE_HPP_

#include "BaseLetter.hpp"

// Represents the End-of-phrase (Eop), and acts as a Base class for
// class EopString.  Contains a count of the number of instances of
// the end-of-phrase in the Corpus.
// Indicates that it's parent letter is an end-of-phrase.
// For example the letter 'y' in the phrase "you must file taxes yearly"
// would contain a sub-tending class Eop with the letter "."
//
// If the phrase "you must file taxes yearly or be arrested"
// also existed, there would be an additional EopString class under the
// letter 'y' with character ":" and m_suffix of " or be arrested".
//
// m_count must be greater than zero.
// m_c must be EOP_VAL.

class EndOfPhrase : public BaseLetter
{
public:
   static constexpr char EOP='.';
   static constexpr char EOP_SUFFIX=':';
   static constexpr char CLASS_NAME[] = "EndOfPhrase";
   static size_t m_totalEOPs;

   EndOfPhrase() : BaseLetter( EOP ), m_count( 1 ) {}
   EndOfPhrase( uint16_t count ) : BaseLetter( EOP ), m_count( count ) {}
   virtual ~EndOfPhrase() {}

   virtual inline const char* getClassName() { return CLASS_NAME; }
   inline uint16_t getCount() const { return m_count; }
   inline void increment(uint16_t count = 1) { m_count = m_count + count; }

   virtual void print( std::ostream& ios, bool printTree, uint8_t level ) const;
   virtual void validate() const;

protected:
   EndOfPhrase( bool isSuffix, uint16_t count ) : BaseLetter( EOP_SUFFIX ), m_count( count )
      {}
private:
   uint16_t m_count;
};



#endif /* ENDOFPHRASE_HPP_ */
