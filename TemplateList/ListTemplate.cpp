/*
 * ListTemplate.cpp
 *
 *  Created on: Aug 28, 2019
 *      Author: L.Burley
 */

#include "ListTemplate.h"
#include "Person.h"

template<class T> SimpleList<T>::SimpleList() :
      m_head( NULL ), m_tail( NULL )
{
}

template<class T> SimpleList<T>::SimpleList( const T& data )
{
   static_cast<Printable*>((T*)0);  // compile time check; ensure that T inherits from Printable
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
      T nodeData = elementPtr->getData();
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


// need to add the line below to support SimpleList Template definitions in a .cpp instead of a .h file.
// C++ 17 supports export of templates, which is another technique.
template class SimpleList<Person>;

// The line below fails ( as expected) during compile since SSN does not inherit from class Printable
//template class SimpleList<SSN>;

