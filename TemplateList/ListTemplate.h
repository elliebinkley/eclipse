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

#define SUCCESS 0
#define DUPLICATE 1
#define NOTFOUND 2

// returns greater of two values; uses a function template.
template<typename T>
inline T const& Max( T const& a, T const& b )
{
   return a < b ? b : a;
}

class Printable
{
   public:
      virtual void print() const = 0;
      virtual void print(std::ostream& s) const = 0;
};

// implements a doubly linked list template class.
template<class T> class SimpleList
{
public:
   class Element
   {
      friend class SimpleList;  // enables SimpleList to call private methods in Element
   public:

      inline T getData()
      {
         return m_data;
      }

   private:
      inline Element( const T& data ) :
            m_data( data ), m_next( NULL ), m_previous( NULL )
      {
      }
      inline Element( const Element& element ) :
            m_data( element.m_data ), m_next( element.m_next ), m_previous( element.m_previous )
      {
      }
      Element();
      inline Element* getNext()
      {
         return m_next;
      }
      inline Element* getPrevious()
      {
         return m_previous;
      }
      inline void setNext( Element* next )
      {
         m_next = next;
      }
      inline void setPrevious( Element* previous )
      {
         m_previous = previous;
      }
      inline void print() const
      {
             m_data.print();
      }

      T m_data;
      Element* m_next;
      Element* m_previous;
   };

   SimpleList();
   SimpleList( const T& element );
   ~SimpleList();
   int addTail( const T& element );
   int addHead( const T& element );
   int remove( const T& element );
   int find( const T& element ) const;
   void print() const;

   inline Element* getHead()
   {
      return m_head;
   }
   inline Element* getTail()
   {
      return m_tail;
   }
   inline Element* getNext( Element* e )
   {
      return e->getNext();
   }
   inline Element* getPrevious( Element* e )
   {
      return e->getPrevious();
   }

private:
   Element* m_head;
   Element* m_tail;
};


#endif
