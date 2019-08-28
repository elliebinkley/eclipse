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

#include <exception>
#include <iostream>
#include <vector>
#include <array>
#include <cassert>

#include "ListTemplate.h"
#include "Person.h"

using namespace std;

void AppUser::useListTemplate()
{
   try
   {
      SimpleList<Person> *list = new SimpleList<Person>;

      Person* larry = new Person( "larry", "burley", "204 Sanford Ave baltimore MD",
            *(new SSN( "215768210" )) );
      Person* cathy = new Person( "cathy", "burley", "4243 S Lincoln St, Denver CO ",
            new SSN( 111, 22, 3333 ) );
      Person* james = new Person( "james", "burley", "1111 Sun Meadow St Hoghlands Ranch CO",
            new SSN( "123456789" ) );

      assert( list->addTail( *larry) == SUCCESS );
      assert( list->addHead( *cathy ) == SUCCESS );
      assert( list->addTail( *james ) == SUCCESS );
      list->print();
      std::cout << " TestId:1 " << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      assert( list->remove( *larry ) == SUCCESS );
      list->print();
      std::cout << " TestId:2" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      assert( list->remove( *cathy ) == SUCCESS );
      list->print();
      assert( list->remove( *james ) == SUCCESS );
      list->print();
      std::cout << " TestId:3" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      delete larry;
      delete cathy;
      delete james;
      delete list;
      std::cout << " TestId:4" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      SimpleList<Person> list1;

      Person jim = Person( "jim", "west", "111 Main st wild west USA", *(new SSN( "222335444" )) );
      Person giselle = Person( "giselle", "bunchen", "new england", new SSN( 123, 24, 7890 ) );
      Person eric = Person( "eric", "balwin", "1234 SNL drive", new SSN( "109768210" ) );
      assert( list1.addHead( jim ) == SUCCESS );
      assert( list1.addTail( giselle ) == SUCCESS );
      assert( list1.addTail( eric ) == SUCCESS );
      list1.print();

      std::cout << " TestId:5" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list1.remove( jim ) == SUCCESS ) ;
      assert( list1.remove( giselle ) == SUCCESS) ;
      list1.print();
      assert ( list1.remove( eric ) == SUCCESS );
      list1.print();
      std::cout << " TestId:6" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      SimpleList<Person> *list3 = new SimpleList<Person>;
      assert( list3->addHead( jim ) == SUCCESS );
      assert( list3->addHead( jim ) == DUPLICATE );
      assert( list3->find( jim ) == SUCCESS );
      assert( list3->remove( jim ) == SUCCESS );
      assert( list3->find( jim ) == NOTFOUND );
      assert( list3->addTail( giselle ) == SUCCESS);
      assert( list3->addTail( eric ) == SUCCESS );
      list3->print();
      std::cout << " TestId:7" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      //  test function template Max()
      SSN tmp=Max(eric.getSSN(), giselle.getSSN());
      std::cout <<  tmp.getSSN()  << std::endl;
      std::cout << " TestId:8" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      // This fails to compile ( as expected)  since list3 requires Persons to be added to it.
      // list3->addTail(tmp);

      std::cout << "Finished" << std::endl;
   }
   catch( const std::exception& e )
   {
      cout << e.what() << endl;
   }
}


