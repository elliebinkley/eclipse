/*
 * ListTemplate.cpp
 *
 *  Created on: Aug 28, 2019
 *      Author: L.Burley
 */

#include <string.h>
#include <cassert>
#include "ListTemplate.h"
#include "Person.h"

using namespace std;


template<class T> SimpleList<T>::SimpleList(size_t n) :
      m_head( NULL ), m_tail( NULL ), m_headFree( NULL), m_tailFree( NULL )
{
   // compile time check; ensure that T inherits from Printable
   // as only objects of type Printable can go in the list.
   static_cast<Printable*>((T*)0);

   initMem(n);
}

template<class T> SimpleList<T>::SimpleList( const T& data, size_t n ):
        m_head( NULL ), m_tail( NULL ), m_headFree( NULL), m_tailFree( NULL )
{
   // compile time check; ensure that T inherits from Printable
   // as only objects of type Printable can go in the list.
   static_cast<Printable*>((T*)0);

   initMem(n);
   addHead(data);
}

template<class T> void SimpleList<T>::initMem( size_t n )
{
   if ( n==0 || n > 2000 ) n=20;
   size_t sz = n * sizeof(Element);
   void* mem= malloc( sz );
   assert( mem );
   memset(  mem, 0, sz );

   // Add blocks to the free list
   Element *e = (Element*) mem;
   m_headFree = 0;
   m_tailFree = 0;
   for( unsigned int i=0; i<n; i++ )
   {
      e->m_next = 0;
      e->m_previous = m_tailFree;
      if( m_tailFree ) m_tailFree->m_next = e;
      m_tailFree = e;
      if( !m_headFree ) m_headFree = e;
      e++;
   }
}

template<class T> SimpleList<T>::~SimpleList()
{
   cout << "dtor SimpleList" << endl;
   // don't assume elements are contiguous;
   // destroy each element one by one.
   while( m_head )
   {
      Element* e=m_head;
      m_head=e->getNext();
      if( m_head ) m_head->setPrevious(NULL);
      if( m_tail == e) m_tail=NULL;
      delete e;
   }
}

// add to the tail of list if data is not already on the list
// if already there return DUPLICATE ;
template<class T> int SimpleList<T>::addTail( const T& data )
{
   Element* e = m_head;

   while( e )
   {
      T nodeData = e->getData();
      if( nodeData == data )
      {
         return 1; // duplicate
      }
      e = e->getNext();
   }
   // no dups

   Element* newNode = (Element*) rmHeadFree();
   if ( !newNode )
   {
      std::cerr << "out of allocated members" << std::endl;
      return NOMEMORY;
   }
   newNode = new( newNode) Element( data );    //  run constructor on allocated memory
   if( !m_head ) m_head = newNode;
   if( m_tail ) m_tail->setNext( newNode );
   newNode->setPrevious( m_tail );
   m_tail = newNode;
   return SUCCESS;
}

// add to the tail of list if data is not already on the list
// if already there return DUPLICATE;
template<class T> int SimpleList<T>::addHead( const T& data )
{

   Element* e = m_head;
   while( e )
   {
      T nodeData = e->getData();
      if( nodeData == data )
      {
         return DUPLICATE;
      }
      e = e->getNext();
   }
   // no dups, so continue...
   Element* newNode = (Element*) rmHeadFree();
   if( !newNode )
   {
      std::cerr << "out of allocated members" << std::endl;
      return NOMEMORY;
   }
   newNode = new( newNode) Element( data );    //  run constructor on allocated memory

   if( !m_tail ) m_tail = (newNode);
   if( m_head ) m_head->setPrevious( newNode );
   newNode->setNext( m_head );
   m_head = newNode;
   return SUCCESS;
}

// removes element in the list with matching data
// Puts it back on free list.
template<class T> int SimpleList<T>::remove( const T& data )
{
   Element* e = m_head;
   while( e )
   {
      T nodeData = e->getData();
      if( nodeData == data )
      {
         Element* nextElement = e->getNext();
         Element* previousElement = e->getPrevious();
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
         this->addTailFree(e);

         // clear old data ...
         memset ( &e->m_data, 0, sizeof(T) );
         return SUCCESS;
      }
      e = e->getNext();
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

template<class T> void SimpleList<T>::addTailFree( Element* e )
{
   e->setPrevious(m_tailFree);
   m_tailFree = e;
   e->setNext(NULL);
   if( !m_headFree) m_headFree=e;
}

// TODO: get more memory if free list is empty.
template<class T> typename SimpleList<T>::Element* SimpleList<T>::rmHeadFree()
{
   SimpleList::Element* e = m_headFree;
   if( e )
   {
      m_headFree=e->getNext();
      m_headFree->setPrevious(NULL);
   }
   return e;
}

template<class T> void SimpleList<T>::print() const
{
   Element* e = m_head;
   while( e )
   {
      e->print();
      e = e->getNext();
   }
   return;
}


// need to add the line below to support SimpleList Template definitions in a .cpp instead of a .h file.
// C++ 17 supports export of templates, which is another technique that could be used.
template class SimpleList<Person>;

// The line below fails ( as expected) during compile since SSN does not inherit from class Printable
// template class SimpleList<SSN>;

