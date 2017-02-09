//============================================================================
// Name        : C++11.cpp
// Author      : larry Burley
// Version     :
// Copyright   : Your copyright notice
// Description : Code for various C++11 features. See individual modules for details
//============================================================================

#include <iostream>
#include <exception>

#include "Utilities.hpp"
#include "UniformInitializationSyntax.hpp"
#include "Construction.hpp"
#include "SmartPointer.hpp"

using namespace std;

int
main ()
{
  T_START;
  try
    {
      UniformInit::UniformInitializationSyntax::getInstance ()->runTests ();
      Construct::Construction::getInstance ()->runTests ();
      SmartPtr::SmartPointer::getInstance ()->runTests ();
      T_END;
    }
  catch (exception& e)
    {
      cerr << e.what () << endl;
    }
  return 0;
}
