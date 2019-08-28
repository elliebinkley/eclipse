/*
 * appUser.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: USER
 *      A little program that demonstrates implementing a doubly linked list by
 *      a custom template implementation.
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
            new SSN( 111, 222, 333 ) );
      Person* james = new Person( "james", "burley", "1111 Sun Meadow St Hoghlands Ranch CO",
            new SSN( "1234567878" ) );

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

      Person jim = Person( "jim", "west", "111 Main st wild west USA", *(new SSN( "2223334444" )) );
      Person giselle = Person( "giselle", "bunchen", "new england", new SSN( 123, 245, 7890 ) );
      Person eric = Person( "eric", "balwin", "1234 SNL drive", new SSN( "10987654321" ) );
      std::cout << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list1.addHead( jim ) == SUCCESS );
      assert( list1.addTail( giselle ) == SUCCESS );
      assert( list1.addTail( eric ) == SUCCESS );
      list1.print();

      std::cout << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;
      assert( list1.remove( jim ) == SUCCESS ) ;
      assert( list1.remove( giselle ) == SUCCESS) ;
      list1.print();
      assert ( list1.remove( eric ) == SUCCESS );
      list1.print();
      std::cout << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      SimpleList<Person> *list3 = new SimpleList<Person>;
      assert( list3->addHead( jim ) == SUCCESS );
      assert( list3->addHead( jim ) == DUPLICATE );
      assert( list3->find( jim ) == SUCCESS );
      assert( list3->remove( jim ) == SUCCESS );
      assert( list3->find( jim ) == NOTFOUND );
      assert( list3->addTail( giselle ) == SUCCESS);
      assert( list3->addTail( eric ) == SUCCESS );
      list3->print();
      std::cout << " TestId:5" << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

      std::cout << "Finished" << std::endl;
   }
   catch( const std::exception& e )
   {
      cout << e.what() << endl;
   }
}


