/*
 * AppUser.h
 *
 *  Created on: Nov 20, 2016
 *      Author: USER
 */

#ifndef APPUSER_H_
#define APPUSER_H_

#include "ListTemplate.h"
#include "Person.h"

// 1. testListTemplateOfPersons(): Tests the SimpleList homemade template ( doubly linked list with its own memory allocation )
//    as an application might. creates a bunch of objects of type People and adds/subtracts them to a SimpleList template.
// 2. CestSTL_ListOfPersons(): Shows basic usage of the STL List template.
//
class AppUser
{
public:
  void testListTemplateOfPersons();
  void testSTL_ListOfPersons();
};

#endif /* APPUSER_H_ */
