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

#include "ListTemplate.h"
#include "Person.h"

using namespace std;

void AppUser::useListTemplate() {
	try {

		SimpleList<Person> *list = new SimpleList<Person>;

		Person* larry = new Person("larry", "burley",
				"204 Sanford Ave baltimore MD", *(new SSN("215768210")));
		Person* cathy = new Person("cathy", "burley",
				"4243 S Lincoln St, Denver CO ", new SSN(111, 222, 333));
		Person* james = new Person("james", "burley",
				"1111 Sun Meadow St Hoghlands Ranch CO", new SSN("1234567878"));

		list->add(*larry);
		list->add(*cathy);
		list->add(*james);
		printList(*list);
		list->remove(*larry);
		printList(*list);
		list->remove(*cathy);
		printList(*list);
		list->remove(*james);
		printList(*list);

		std::cout << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

		delete larry;
		delete cathy;
		delete james;

		delete list;

		std::cout << " File=" << __FILE__ << " line=" << __LINE__ << std::endl;

		SimpleList<Person> list1;

		Person jim = Person("jim", "west", "111 Main st wild west USA",
				*(new SSN("2223334444")));
		Person giselle = Person("giselle", "bunchen", "new england",
				new SSN(123, 245, 7890));
		Person eric = Person("eric", "balwin", "1234 SNL drive",
				new SSN("10987654321"));
		std::cout << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;
		list1.add(jim);
		list1.add(giselle);
		list1.add(eric);
		std::cout << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;
		printList(list1);
		list1.remove(jim);
		list1.remove(giselle);
		printList(list1);
		list1.remove(eric);
		printList(list1);
		std::cout << "File=" << __FILE__ << " line=" << __LINE__ << std::endl;
	} catch (const std::exception& e) {
		cout << e.what() << endl;
	}
}

void AppUser::printList(SimpleList<Person>& list) {
	SimpleList<Person>::Element* e = list.getHead();
	cout << "Printing list" << endl;
	while (e) {
		Person p = e->getData();
		p.print();
		e = e->getNext();
	}
}

