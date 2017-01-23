/*
 * ListUser.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: USER
 *      A little program that demonstrates implementing a doubly linked list by
 *      a custom template implementation.
 */

#include <iostream>
#include <string>
#include <cstdlib>

#include "AppUser.h"

using namespace std;

int main(void) {
	cout << "starting main File=" << __FILE__ << "  line=" << __LINE__ << endl;
	AppUser appUser;
	appUser.useListTemplate();
}

