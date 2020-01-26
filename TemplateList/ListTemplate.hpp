/*
 * ListTemplate.h
 *
 *  Created on: Nov 16, 2016
 *      Author: L.Burley
 *      Implement a doubly linked list via templates.
 */

#ifndef SIMPLELIST_H_
#define SIMPLELIST_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "Printable.hpp"

// returns greater of two values; uses a function template.
template<typename T>
inline T const& Max( T const& a, T const& b )
{
   return a < b ? b : a;
}

// implements a doubly linked list template class.
//       1.  Instantiation allocates memory at startup for N units.
//       2.  addTail(), addHead() return DUPLICATE if already there
//       3.  addTail(), addHead() return NOMEMORY if allocation exceeded.
//       4.  addTail(), addHead() make a copy of the data passed in
//           using the object<T> copy constructor.
//       4.  find()    returns NOTFOUND if not found, else SUCCESS.  Returns a
//           pointer to read-only element on the list.
//       5.  remove()  returns NOTFOUND if not found, else SUCCESS if removed.
//       6.  remove() runs the DTOR on the found element.
// Todo: 1.  write an iterator.
//       3.  Make multithreaded, protect with mutex  and/or atomic.
//       4.  Make expandable beyond max units.
//       5.  Test with C++  unit test.

template<class T> class SimpleList
{
public:
   enum ListAction { SUCCESS, DUPLICATE, NOTFOUND, NOMEMORY };

   // Element is an inner class with outer class as a friend.
   class Element : public Printable
   {
      friend class SimpleList;  // enables SimpleList to call private methods in Element
   public:

      inline T getData()
      {
         return m_data;  // returns a copy.
      }

      virtual ~Element()
      {
      }

      inline void print() const
      {
         m_data.print();
      }

      inline void print( std::ostream& s ) const
      {
         m_data.print( s );
      }

      inline void printDebug( std::ostream& s ) const
      {
         m_data.print();
         s << "next=" << this->m_next << " previous=" << this->m_previous << std::endl;
      }
   private:
      // CTORS are private..
      inline Element( const T& data ) :
            m_data( data ), m_next( NULL ), m_previous( NULL )
      {
      }

      inline Element( const Element& element ) :
            m_data( element.m_data ), m_next( element.m_next ), m_previous( element.m_previous )
      {
      }

      Element();
      // list traversal is private
      inline Element* getNext()
      {
         return m_next;
      }

      inline void setNext( Element* next )
      {
         m_next = next;
      }

      inline Element* getPrevious()
      {
         return m_previous;
      }

      inline void setPrevious( Element* previous )
      {
         m_previous = previous;
      }

      // Member data
      T m_data;
      Element* m_next;
      Element* m_previous;
   };

   SimpleList( size_t n = 20 );
   SimpleList( const T& data, size_t n = 20 );
   ~SimpleList();
   int addTail( const T& data );
   int addHead( const T& data );
   int remove( const T& data );
   int find( const T& data ) const;
   Element* const findElement( const T& data ) const;

   void print() const;

   inline Element* const getHead()
   {
      return m_head;
   }
   inline Element* const getTail()
   {
      return m_tail;
   }
   inline Element* const getNext( Element* e )
   {
      return e->getNext();
   }
   inline Element* const getPrevious( Element* e )
   {
      return e->getPrevious();
   }

private:
   // memory allocation for n elements
   void initMem( size_t n );

   // free list functions..
   void addTailFree( Element* element );
   Element* rmHeadFree();

   Element* m_head;
   Element* m_tail;
   Element* m_headFree;
   Element* m_tailFree;
   void*    m_memory;  // total pool of memory
};

#endif
