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

class AppUser
{
public:
  void
  useListTemplate ();
  void
  printList (SimpleList<Person>& list);
};

#endif /* APPUSER_H_ */
