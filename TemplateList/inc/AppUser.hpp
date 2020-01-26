/*
 * AppUser.h
 *
 *  Created on: Nov 20, 2016
 *      Author: USER
 */

#ifndef INC_APPUSER_HPP_
#define INC_APPUSER_HPP_

#include "../inc/ListTemplate.hpp"
#include "../inc/Person.hpp"

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

#endif /* INC_APPUSER_HPP_ */
