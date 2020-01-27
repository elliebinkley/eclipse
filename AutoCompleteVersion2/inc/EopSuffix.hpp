/*
 * EopSuffix.hpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#ifndef EOPSUFFIX_HPP_
#define EOPSUFFIX_HPP_

#include "EndOfPhrase.hpp"

// Purpose: Stores the last unique string suffix of the phrase.
//
// The letter for EopString class is always character ':' ( semicolon).
// It indicates that it's parent letter has a unique suffix ending the phrase.
// Example: e.g.
// Assume phrases "you must pay Marie Foster for the chocolate cake" and
// "you must pay Marge Smith for the window pane".
// For the last letter "i" in "you must pay Mari"
// an EopString with character ':' containing a m_suffix of
// "e Foster for the chocolate cake" would exist.
// For the last letter g in "you must pay Marg"
// an EopString with character ':' and m_suffix of
// "e Smith for the window pane" would exist.

class EopSuffix : public EndOfPhrase
{
public:
   static constexpr char CLASS_NAME[] = "EopSuffix";
   static size_t m_totalEopSuffix;

   EopSuffix() : EndOfPhrase(true,1), m_suffix("") {}
   EopSuffix( const std::string& phrase, uint16_t index, uint16_t count );
   virtual ~EopSuffix()  {}

   virtual inline const char* getClassName() { return CLASS_NAME; }
   inline const std::string& getSuffix() { return m_suffix; }
   virtual void print( std::ostream& ios, bool printTree, uint8_t level) const;
   virtual void validate() const;
   virtual inline const char* getType() { return CLASS_NAME; }

private:
   std::string m_suffix;
};




#endif /* EOPSUFFIX_HPP_ */
