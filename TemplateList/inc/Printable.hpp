/*
 * Printable.hpp
 *
 *  Created on: Nov 8, 2019
 *      Author: laurence
 */

#ifndef INC_PRINTABLE_HPP_
#define INC_PRINTABLE_HPP_

#include <ostream>

class Printable
{
public:
   virtual void print() const = 0;
   virtual void print( std::ostream& s ) const = 0;
   inline virtual ~Printable() = default;
};




#endif /* INC_PRINTABLE_HPP_ */
