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

// implements a doubly linked list template class.
template<class T> class SimpleList
{
public:
   class Element
   {
      friend class SimpleList;
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
             std::cout << "name:" << this << std::endl;
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

/*
template<class T> SimpleList<T>::SimpleList() :
      m_head( NULL ), m_tail( NULL )
{
}

template<class T> SimpleList<T>::SimpleList( const T& data )
{
   m_head = new Element( data );
   m_tail = m_head;
}

template<class T> SimpleList<T>::~SimpleList()
{
   while( m_head )
   {
      m_head->print();
      Element* elementPtr=m_head;
      m_head=elementPtr->getNext();
      if( m_head )  m_head->setPrevious(NULL);
      if( m_tail == elementPtr) m_tail=NULL;
      delete elementPtr;
   }
}

// add to the tail of list if data is not already on the list
// if already there return 1;
template<class T> int SimpleList<T>::addTail( const T& data )
{
   Element* elementPtr = m_head;
   while( elementPtr )
   {
      T nodeData = elementPtr->getData();
      if( nodeData == data )
      {
         return 1; // duplicate
      }
      elementPtr = elementPtr->getNext();
   }
   // no dups
   Element* newNode = new Element( data );
   if( !m_head ) m_head = newNode;
   if( m_tail ) m_tail->setNext( newNode );
   newNode->setPrevious( m_tail );
   m_tail = newNode;
   return 0;
}
// add to the tail of list if data is not already on the list
// if already there return 1;
template<class T> int SimpleList<T>::addHead( const T& data )
{
   Element* elementPtr = m_head;
   while( elementPtr )
   {
      T nodeData = elementPtr->getData();
      if( nodeData == data )
      {
         return 1; // duplicate
      }
      elementPtr = elementPtr->getNext();
   }
   // no dups
   Element* newNode = new Element( data );
   if( !m_tail ) m_tail = (newNode);
   if( m_head ) m_head->setPrevious( newNode );
   newNode->setNext( m_head );
   m_head = newNode;
   return SUCCESS;
}

// removes element in the list with matching data
template<class T> int SimpleList<T>::remove( const T& data )
{
   Element* elementPtr = m_head;
   while( elementPtr )
   {
      T nodeData = elementPtr->getData();
      if( nodeData == data )
      {
         Element* nextElement = elementPtr->getNext();
         Element* previousElement = elementPtr->getPrevious();
         if( previousElement ){
            previousElement->setNext( nextElement );
         }
         else // deleting head
         {
            m_head=nextElement;
         }
         if( nextElement )
         {
            nextElement->setPrevious( previousElement );
         }
         else // // deleting tail
         {
            m_tail=previousElement;
         }
         delete elementPtr;
         return SUCCESS;
      }
      elementPtr = elementPtr->getNext();
   }
   return NOTFOUND;
}

template<class T> int SimpleList<T>::find( const T& data ) const
{
   Element* elementPtr = m_head;
   while( elementPtr )
   {
      T nodeData = (*elementPtr)->getData();
      if( nodeData == data )
      {
         return SUCCESS;
      }
      elementPtr = elementPtr->getNext();
   }
   return NOTFOUND;
}

template<class T> void SimpleList<T>::print() const
{
   Element* elementPtr = m_head;
   while( elementPtr )
   {
      elementPtr->print();
      elementPtr = elementPtr->getNext();
   }
   return;
}
*/
#endif
