/*
 * ListUser.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: Burley
 *      A little program that demonstrates implementing a doubly linked list by
 *      a custom template implementation.
 *      Also shows use of <list> STL
 */

#include "AppUser.h"

using namespace std;

int main( void )
{
    cout << "starting main File=" << __FILE__ << "  line=" << __LINE__ << endl;

    AppUser appUser;

    // Tests the simple homemade linked list template class.
    appUser.testListTemplateOfPersons();

    // Run a test that uses the STL linked link

    appUser.testSTL_ListOfPersons();

}
