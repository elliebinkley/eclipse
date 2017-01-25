/*
 * ListTemplate.h
 *
 *  Created on: Nov 16, 2016
 *      Author: USER
 *      Implement a doubly linked list via templates.
 */

#ifndef SIMPLELIST_H_
#define SIMPLELIST_H_

#include <iostream>
#include <string>

// returns greater of two values; uses a function template.
template<typename T>
  inline T const&
  Max (T const& a, T const& b)
  {
    return a < b ? b : a;
  }

// implements a doubly linked list template class.
template<class T>
  class SimpleList
  {
  public:
    class Element
    {
    public:
      inline
      Element (const T& data) :
	  m_data (data), m_next (NULL), m_previous (NULL)
      {
      }
      inline
      Element (const Element& element) :
	  m_data (element.m_data), m_next (element.m_next), m_previous (
	      element.m_previous)
      {
      }
      inline T
      getData ()
      {
	return m_data;
      }
      inline Element*
      getNext ()
      {
	return m_next;
      }
      inline Element*
      getPrevious ()
      {
	return m_previous;
      }
      inline void
      setNext (Element* next)
      {
	m_next = next;
      }
      inline void
      setPrevious (Element* previous)
      {
	m_previous = previous;
      }
    private:
      T m_data;
      Element* m_next;
      Element* m_previous;
    };

    SimpleList ();
    SimpleList (const T& element);
    ~SimpleList ();
    int
    add (const T& element);
    int
    remove (const T& element);
    T
    find (const T& element) const;
    inline Element*
    getHead ()
    {
      return m_head;
    }
    inline Element*
    getNext (Element* e)
    {
      return e->getNext ();
    }

  private:

    Element* m_head;
  };

template<class T>
  SimpleList<T>::SimpleList () :
      m_head (NULL)
  {
  }

template<class T>
  SimpleList<T>::SimpleList (const T& data)
  {
    m_head = new Element (data);
  }
template<class T>
  SimpleList<T>::~SimpleList ()
  {
    while (m_head)
      {
	remove (m_head->getData ());
      }
  }

// add to the list if data is not already on the list
template<class T>
  int
  SimpleList<T>::add (const T& data)
  {
    Element* newNode = new Element (data);
    Element* elementPtr = m_head;
    if (m_head)
      {
	while (elementPtr)
	  {
	    T nodeData = elementPtr->getData ();
	    if (nodeData == data)
	      return 1; // duplicate
	    if (!elementPtr->getNext ())
	      break;
	    elementPtr = elementPtr->getNext ();
	  }
	elementPtr->setNext (newNode);
      }
    else
      {
	m_head = newNode;
      }

    newNode->setPrevious (elementPtr);

    return 0;
  }

// removes all elements in the list with matching data
template<class T>
  int
  SimpleList<T>::remove (const T& data)
  {
    int retVal = 1;
    Element* elementPtr = m_head;
    while (elementPtr)
      {
	T nodeData = elementPtr->getData ();
	if (nodeData == data)
	  {
	    // remove the node
	    Element* nextElement = elementPtr->getNext ();
	    Element* previousElement = elementPtr->getPrevious ();
	    if (!previousElement && !nextElement)
	      {
		// only one element
		m_head = 0;
	      }
	    if (!previousElement && nextElement)
	      {
		// head of the list; but not the only one.
		m_head = nextElement;
	      }

	    if (nextElement)
	      {
		nextElement->setPrevious (previousElement);
	      }
	    if (previousElement)
	      {
		previousElement->setNext (nextElement);
	      }

	    delete elementPtr;
	    elementPtr = nextElement;
	    retVal = 0; // continue to eliminate duplicates ( should not happen )
	  }
	else
	  {
	    elementPtr = elementPtr->getNext ();
	  }

      }
    return retVal;
  }

template<class T>
  T
  SimpleList<T>::find (const T& data) const
  {
    Element* elementPtr = m_head;
    while (elementPtr)
      {
	T nodeData = (*elementPtr)->getData ();
	if (nodeData == data)
	  {
	    return nodeData;
	  }
	elementPtr = elementPtr->getNext ();
      }
    return NULL;
  }

#endif
