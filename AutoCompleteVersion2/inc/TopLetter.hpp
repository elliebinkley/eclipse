/*
 * TopLetter.hpp
 *
 *  Created on: Oct 29, 2019
 *      Author: laurence
 */

#ifndef TOPLETTER_HPP_
#define TOPLETTER_HPP_

#include "Letter.hpp"
#include <string_view>

// This class is a top level holder; it doesn't have a letter value,
// just a char value of '$' to indicate a top_level node in the tree.
// In may contain one or more Letters, but no other TopLetters can exist.
//
// Subtending letters are in m_lettermap
// m_lettermap may contain:
// 1. 1-29 total entries
// 2. zero or one Eop classes,
// 3. zero or one EopSuffix classes,
// 4. 1-29 letter classes.
// 5. EopSuffix and Letter instances are mutually exclusive, i.e.
//    if one EopSuffix exist, then no letter classes exist and
//    if one or more letters exist, then no EopSuffix exists.
// 6. No TopLetters.
// 7. entries should be unique and sorted by m_c.
// 8. validate() checks these rules

class TopLetter  : public Letter
{
public:
   static constexpr char TOP_LETTER = '$';
   static constexpr char CLASS_NAME[] = "TopLetter";

   TopLetter() : Letter(TOP_LETTER) {};
   virtual ~TopLetter() = default;

   virtual inline const char* getClassName() { return CLASS_NAME; }
   virtual inline std::string_view getClassNameView() { return CLASS_NAME; }
   virtual void validate() const;
private:
};

#endif /* TOPLETTER_HPP_ */
