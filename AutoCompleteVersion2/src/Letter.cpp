/*
 * Letter.cpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#include "../inc/BoostMacros.hpp"
#include "../inc/Config.hpp"
#include "../inc/EndOfPhrase.hpp"
#include "../inc/EopSuffix.hpp"
#include "../inc/Letter.hpp"
#include "../inc/TopLetter.hpp"
#include "../inc/FoundItemsImplTrie.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <typeinfo>

size_t Letter::m_totalLetters = 0;

/* Increment the counter on an EOP due to a duplicate encounter during
 * Corpus construction.
 */
void Letter::incrementEop(uint16_t count)
{
   // find via binary search, then increment the counter.
   // else insert into sorted vector with the passed counter.
   BL_Container::iterator it =
         std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                          EndOfPhrase::EOP, keyCompare() );
   if( it != m_lettermap.end() && it->get()->getChar() == EndOfPhrase::EOP )
   {
      // already present
      EndOfPhrase* eop = static_cast<EndOfPhrase*>( it->get() );
      eop->increment( count );
   }
   else
   {
      m_lettermap.insert(it, std::make_unique<EndOfPhrase>( count )  );
      this->setEop( true );
   }
   return;
}

/*
 * If an EndOfPhrase exists, add it to the FoundItems result map
 */
void Letter::addEopToResults( std::string& path, FoundItemsImplTrie& results)
{
   assert ( hasEop() ); // assume caller checked for Eop prior to call.
   // binary search
   BL_Container::iterator itEop =
         std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                          EndOfPhrase::EOP, keyCompare() );
   if( itEop != m_lettermap.end() )
   {
      assert( itEop->get()->getChar() == EndOfPhrase::EOP );
      EndOfPhrase* eop = static_cast<EndOfPhrase*>( itEop->get() );
      results.insert( path,  eop->getCount() );
      GEN_DBG2("Eop added:", path );
   }
   return;
}

/*
 *  A suffix exists, see if it should be added to the result map.
 */
void Letter::addEopSuffixToResults( std::string& path, const std::string& query, FoundItemsImplTrie& results)
{
   assert( hasEopSuffix() );  // assume caller checked for this
   BL_Container::iterator itEopSuffix =
         std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                          EndOfPhrase::EOP_SUFFIX, keyCompare() );
   if( itEopSuffix != m_lettermap.end() )
   {
      assert( itEopSuffix->get()->getChar() == EndOfPhrase::EOP_SUFFIX );
      // found a suffix;
      EopSuffix* eopSuffix = static_cast<EopSuffix*>( itEopSuffix->get() );


      if ( query.size() > path.size() )
      {
         // if path is shorter than the query string
         // then there might still be a match if
         // the path + first part of eopSuffix string matches the query.
         // E.g. query="america" and path is at node "amer";
         // and there is a EOPsuffix string "icana"; so it's a match.
         size_t count = query.size() - path.size(); // number of letters to compare.
         if( !query.compare(path.size(), count, eopSuffix->getSuffix(), 0, count ) == 0 )
         {
            std::cout << "no match" << std::endl;
            return;
         }
         else
         {
            std::cout << "match" << std::endl;
         }
      }

      results.insert( path + eopSuffix->getSuffix(), eopSuffix->getCount() );
      GEN_DBG2("EopSuffix added:", path + eopSuffix->getSuffix() );
   }
   return;
}

// Collect any matching phrases under this letter and put them in results.
// Note that parameter path is the current path in the corpus tree,
// not including this letter. The query parameter is what the user specified in the search.
// Note that the parameter "path" may be shorter or longer than
// the query; it will be longer if phrase collection has begin and and shorter if the search hasn't
// reached the collect node.
void Letter::collectPhrases( std::string& path, const std::string& query, FoundItemsImplTrie& results)
{
   GEN_DBG4( "m_c:", getChar()," path:", path );
   bool collect=false;    // collect from lower letters
   bool search=false;    // search the lower letters.

   if( getChar() != TopLetter::TOP_LETTER)
   {
      // add the letter to the path; Note that topLetter ('$'.) is never part of the path.
      path.push_back( getChar() );
   }

   if ( query.size() <= path.size() ) // deep enough in search to collect results
   {
      if( path.compare(0, query.size(), query) == 0 )
      {
         // query is a substring of path; so collect results.
         // e.g.query = "amer" and path = "america"
         if( hasEop() )  addEopToResults(path, results);
         if( hasEopSuffix() )
         {
            addEopSuffixToResults(path, query, results);
         }
         else
         {
            collect = true;
         }
      }
   }
   else  // path is shorter than quer; e.g.query = "amer" and path = "am"
   {
      // in search mode, not collect mode
      if (query.compare( 0, path.size(), path) == 0 )
      {
         // e.g.query = "amer" and path = "am";
         // See if suffix should be added to results;
         // this is the one case where a search turns into a collect.
         if ( hasEopSuffix() )
         {
            addEopSuffixToResults(path, query, results);
         }
         else
         {
            search = true;
         }
      }
   }
   assert( ! (collect && search ));  // both cannot be true.
   if ( collect )
   {
      // collect phrases from the child letters
      for (const auto& iter : m_lettermap )
      {
         char letter = iter->getChar();
         // see if this is a Letter class
         if ( ( isalpha(letter) || isblank(letter) ) || ( letter == TopLetter::TOP_LETTER ) )
         {
            Letter* letterPtr = static_cast<Letter*>(iter.get());
            letterPtr->collectPhrases( path, query, results );
         }
         else
         {
            assert( letter == EndOfPhrase::EOP );
         }
      }
   }
   else if( search )
   {
      // search the lower levels for the next letter
      // e.g.query = "amer" and path = "am" ; next letter is 'e'.
      char nextChar = query[path.size()];
      BL_Container::iterator itNextLetter =
             std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                              nextChar, keyCompare() );
       if( itNextLetter != m_lettermap.end() )
       {
          assert( itNextLetter->get()->getChar() == nextChar );
          // found it
          Letter* letterPtr = static_cast<Letter*>(itNextLetter->get());
          letterPtr->collectPhrases( path, query, results );
       }
   }

   if( getChar() != TopLetter::TOP_LETTER) path.pop_back();
   GEN_DBG4( "after pop m_c:", getChar()," path:", path );
   return;
}

/*
 * Insert a substring of phrase starting at index of phrase
 * into this Letter's lettermap structure.
 * "count" is how many counts of the phrase to persist; usually it is 1, but
 * when converting a EopSuffix to a set of letters it could be > 1.
 */
void Letter::insert( const std::string& phrase, uint16_t index, uint16_t count )
{
   assert ( index <= phrase.length() );

   // Case 1: add a EndOfPhrase marker, or if EOP exists already, increment EOP dup counter
   if ( index == phrase.length() )
   {
      // insert an Eop or increment the existing Eop.
      incrementEop( count );
      return;
   }

   // Case 2: if an EopSuffix object is present, then if the EopSuffix string contents match
   // the phrase, increment the EopSuffix counter,
   // else, convert the old EopSuffix to a Letter as per rule3 in Letter.hpp
   if ( hasEopSuffix() )
   {
      BL_Container::iterator itEopSuffix =
            std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                             EndOfPhrase::EOP_SUFFIX, keyCompare() );

      if( itEopSuffix != m_lettermap.end() &&
            itEopSuffix->get()->getChar() == EndOfPhrase::EOP_SUFFIX )
      {
         // found a suffix; compare contents
         EopSuffix* eopSuffix = (EopSuffix*)itEopSuffix->get();

         if ( phrase.compare( index, phrase.size(), eopSuffix->getSuffix() ) == 0 )
         {
            // The suffix strings match, so just add more duplicates...
            eopSuffix->increment(count);
            return;
         }

         // The EopSuffix did not match; so now convert eopSuffix to a Letter
         // because of rule #3 in Letter.hpp
         std::string oldPhrase = eopSuffix->getSuffix();  // remember the old phrase
         uint16_t oldCount = eopSuffix->getCount();       // remember the old count
         m_lettermap.erase( itEopSuffix );                // erase the old EOP_SUFFIX
         setEopSuffix( false );

         // re-insert the old phrase as a Letter;
         std::unique_ptr<Letter> ptrOld = std::make_unique<Letter>(  oldPhrase[0] );
         BL_Container::iterator itOldLetter = std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                                                               ptrOld->getChar(), keyCompare() );
         m_lettermap.insert(itOldLetter, std::move(ptrOld) );

         //  recursively add the remaining sub-phrase
         Letter* lowerLetter = (Letter*)itOldLetter->get();
         lowerLetter->insert( oldPhrase, 1, oldCount );
      }
      else
      {
         assert( !hasEopSuffix() );  // bit set but no EopSuffix exists
      }
   }// end of handling converting the EopSuffix to a Letter.

   // Case 3: If the phrase[index] letter already exists,
   // then recursively insert the new sub-phrase in that existing letter.
   if ( m_lettermap.size() != 0 )
   {
      // some letters exist, check if letter phrase[index] exists.
      std::unique_ptr<Letter> ptr = std::make_unique<Letter>( phrase[index] );
      BL_Container::iterator itLetter = std::lower_bound(m_lettermap.begin(), m_lettermap.end(),
                                                         ptr->getChar(), keyCompare() );
      if( itLetter != m_lettermap.end() && itLetter->get()->getChar() == phrase[index] )
      {
         // letter is already present, recursively add the sub-phrase to that letter.
         Letter* lowerLetter = static_cast<Letter*>(itLetter->get());
         lowerLetter->insert(phrase, index + 1, count );
         return;
      }
      else
      {
         // Case 5: phrase[index] Letter is not present; so insert/sort letter into container
         BL_Container::iterator insertIt = m_lettermap.insert(itLetter, std::move( ptr ) );

         //  recursively add the sub-phrase to that newly inserted letter.
         Letter* lowerLetter = static_cast<Letter*>(insertIt->get());
         lowerLetter->insert( phrase, index+1, count );
         return;
      }
   }
   else
   {
      m_lettermap.push_back(std::make_unique<EopSuffix>( phrase, index, count  ));
      setEopSuffix( true );
   }

   return;
}


void Letter::print(std::ostream &ios, bool printTree, uint8_t level ) const
{
   for ( uint8_t indent=0; indent < level; indent++) ios << "  ";
   ios << " Letter=" << this->getChar() << " hasEop=" << hasEop() << " hasEopSuffix=" << hasEopSuffix() << std::endl;

   if ( printTree )
   {
      for ( auto& iter:m_lettermap)
      {
         iter->print(ios, printTree, level + 1 );
      }
   }
}

// validate consistency of Letter class.
// The contents of m_lettermap must have one and only one of the following true:
// 1. Zero or one Eop classes,
// 2. Zero or one EopString classes,
// 3. If one EopString class, then no letter classes exist.
// 4. If one or more letter classes exist, then no EopString classes exist.
// 5. No TopLetters.
// 6. Letters must be a-z,A-Z or a space

void Letter::validate() const
{
   bool isError = false;

   std::stringstream ss;

   if( !isalpha( getChar() ) && !isblank( getChar() ))
   {
      isError = true;
      ss << " error:invalidLetter=" << getChar();
      Config::getInstance()->incrementValidateErrors();
   }

   if( isalpha( getChar() ) && !islower( getChar() ))
   {
      isError = true;
      ss << " error:invalidLetter=" << getChar();
      Config::getInstance()->incrementValidateErrors();
   }

   if ( isError )
   {
      print(ss, false, 0);
      GEN_DBG1( ss.str() );
   }

   m_totalLetters++;

   this->validateChildren();
}

void Letter::validateChildren() const
{
   // run logic on child combinations, then validate each child
   std::stringstream ss;
   uint32_t eopCount = 0;
   uint32_t eopSuffixCount = 0;
   uint32_t letterClassCount = 0;
   bool isError = false;

   // enforce map rules.
   for( const auto &item : m_lettermap )
   {
      char letter = item->getChar ();
      if( letter == EndOfPhrase::EOP_SUFFIX )
      {
         // only zero or one suffix allowed per m_lettermap
         if( ++eopSuffixCount > 1 )
         {
            isError = true;
            ss << " error:eopSuffixCount=" << eopSuffixCount;
            Config::getInstance()->incrementValidateErrors();
         }
         if ( !hasEopSuffix())
         {
            isError = true;
            ss << " error:eofSuffixCount and bit do not match; eopSuffixCount=" << eopSuffixCount;
            Config::getInstance()->incrementValidateErrors();
         }
      }
      else if( letter == EndOfPhrase::EOP )
      {
         // only zero or one EOP allowed per m_lettermap
         if( ++eopCount > 1)
         {
            isError = true;
            ss << " error:eopCount=" << eopCount;
            Config::getInstance()->incrementValidateErrors();
         }
         if ( !hasEop() )
         {
            isError = true;
            ss << " error:eopCount and bit do not match; eopCount=" << eopCount;
            Config::getInstance()->incrementValidateErrors();
         }
      }
      else
      {
         // assume a lower case letter or blank; if not true; later checks will catch.
         letterClassCount++;
      }
   }

   if( eopSuffixCount > 0 && letterClassCount > 0 )
   {
      isError = true;
      ss << " error:eopSuffixCount=" << eopSuffixCount
            << " and letterClassCount=" << letterClassCount;
      Config::getInstance()->incrementValidateErrors();
   }

   if( eopSuffixCount > 1 )
   {
        isError = true;
        ss << " error:eopSuffixCount=" << eopSuffixCount;
        Config::getInstance()->incrementValidateErrors();
   }

   if ( isError )
   {
      print(ss, false, 0);
      GEN_DBG1( ss.str() );
   }

   // validate chilidren
   for( const auto &item : m_lettermap ) item->validate();
   return;

}


