/*
 * appUser.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: USER
 *      A little program that demonstrates implementing a doubly linked list by
 *      a custom template implementationIt also shows a function template.
 *      The elements to the list must inherit from the base class Printable.
 *
 */

#include "appUser.h"

#include <cassert>
#include <list>
#include "ListTemplate.h"
#include "Person.h"

using namespace std;

void AppUser::testListTemplateOfPersons()
{
   try
   {
      int id = 1;
cout << "a0" << endl;
      SimpleList<Person> *list = new SimpleList<Person>;
      list->print();

      Person* larry = new Person( "larry", "burley", "204 Sanford Ave baltimore MD",
            *(new SSN( "215768210" )) );
      Person* cathy = new Person( "cathy", "burley", "4243 S Lincoln St, Denver CO ",
            new SSN( 111, 22, 3333 ) );
      Person* james = new Person( "james", "burley", "1111 Sun Meadow St Hoghlands Ranch CO",
            new SSN( "123456789" ) );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list->addTail( *larry) == SimpleList<Person>::SUCCESS);

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      list->print();

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list->addHead( *cathy ) == SimpleList<Person>::SUCCESS );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list->addTail( *james ) == SimpleList<Person>::SUCCESS );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      list->print();

      std::cout << " TestId: " << id++ << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list->remove( *larry ) == SimpleList<Person>::SUCCESS );
      list->print();

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list->remove( *cathy ) == SimpleList<Person>::SUCCESS );
      list->print();

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list->remove( *james ) == SimpleList<Person>::SUCCESS );
      list->print();

      delete larry;
      delete cathy;
      delete james;
      delete list;

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      SimpleList<Person> list1;
      Person jim = Person( "jim", "west", "111 Main st wild west USA", *(new SSN( "222335444" )) );
      Person giselle = Person( "giselle", "bunchen", "new england", new SSN( 123, 24, 7890 ) );
      Person eric = Person( "eric", "baldwin", "1234 SNL drive", new SSN( "109768210" ) );
      assert( list1.addHead( jim ) == SimpleList<Person>::SUCCESS );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list1.addTail( giselle ) == SimpleList<Person>::SUCCESS );
      assert( list1.addTail( eric ) == SimpleList<Person>::SUCCESS );
      list1.print();

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      SimpleList<Person>::Element* e = list1.getHead();
      while( e )
      {
         e->print();
         e = list1.getNext( e );
      }

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      e = list1.getTail();
      while( e )
      {
         e->print();
         e = list1.getPrevious( e );
      }

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list1.remove( jim ) ==  SimpleList<Person>::SUCCESS );
      assert( list1.remove( giselle ) ==  SimpleList<Person>::SUCCESS);
      list1.print();
      assert( list1.remove( eric ) ==  SimpleList<Person>::SUCCESS );
      list1.print();

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      SimpleList<Person> *list3 = new SimpleList<Person>;
      assert( list3->addHead( jim ) ==  SimpleList<Person>::SUCCESS );
      assert( list3->addHead( jim ) ==  SimpleList<Person>::DUPLICATE );
      assert( list3->find( jim ) ==  SimpleList<Person>::SUCCESS );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list3->remove( jim ) ==  SimpleList<Person>::SUCCESS );
      assert( list3->find( jim ) ==  SimpleList<Person>::NOTFOUND );
      assert( list3->addTail( giselle ) ==  SimpleList<Person>::SUCCESS );
      assert( list3->addTail( eric ) ==  SimpleList<Person>::SUCCESS );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      SimpleList<Person>::Element* element = list3->findElement(giselle);
      assert( element != NULL);
      element=list3->getNext(element);
      assert(element->getData() == eric );
      list3->print();

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      element=list3->getPrevious(element);
      assert(element->getData() == giselle );

      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      //  test function template Max()
      SSN tmp = Max( eric.getSSN(), giselle.getSSN() );
      std::cout << tmp.getSSN() << std::endl;
      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      // This fails to compile ( as expected)  since list3 requires Persons to be added to it.
      // list3->addTail(tmp);

      std::cout << "Finished testing of ListTemplateOfPersons()" << std::endl;
   }
   catch( const std::exception& e )
   {
      cout << e.what() << endl;
   }
}

void AppUser::testSTL_ListOfPersons()
{
   try
   {
      int id=1;

      list<Person> *list_1 = new list<Person>;

      Person jim = Person( "jim", "west", "111 Main st wild west USA", *(new SSN( "222335444" )) );
      Person giselle = Person( "giselle", "bunchen", "new england", new SSN( 123, 24, 7890 ) );
      Person eric = Person( "eric", "baldwin", "1234 SNL drive", new SSN( "109768210" ) );

      list_1->push_back( jim );
      list_1->push_back( giselle );
      list_1->push_back( eric );

      list<Person>::iterator listIterator_1;
      for( listIterator_1 = list_1->begin(); listIterator_1 != list_1->end(); listIterator_1++ )
      {
         listIterator_1->print();
      }
      delete list_1;
      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      Person myPeople[] = { Person( "jim", "west", "111 Main st wild west USA", *(new SSN( "222335444" )) ),
                            Person( "giselle", "bunchen", "new england", new SSN( 123, 24, 7890 ) ),
                            Person( "eric", "baldwin", "1234 SNL drive", new SSN( "109768210" ) ) };
      list<Person> *list_2 = new std::list<Person>(  myPeople , myPeople + 3);

      list_2->get_allocator().allocate(10);   // allocate 10 elements in memory.
      list<Person>::iterator listIterator_2;
      for( listIterator_2 = list_2->begin(); listIterator_2 != list_2->end(); listIterator_2++ )
      {
         listIterator_2->print();
      }
      delete list_2;
      std::cout << " TestId:" << id++ << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      std::cout << "Finished testSTL_ListOfPersons()" << std::endl;
   }
   catch( const std::exception& e )
   {
      cout << e.what() << endl;
   }
}

