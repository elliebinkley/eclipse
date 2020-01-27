/* Class used for testing purposes;
 *  1. create a test file on disk with vehicles
 *  2. read in test file into a simple vector
 *  3. provide simple search by model and year on the simple vector.
 */

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include "../inc/VehicleRepoVector.hpp"
#include "../inc/VehicleFile.hpp"

void VehicleRepoVector::cleanup()
{
   cout << "cleaning up m_repoVector" << endl;
   for( auto iter : m_repoVector )
   {
      m_repoVector.erase( m_repoVector.begin(), m_repoVector.end() );
   }
   assert( m_repoVector.empty() );
}

// initialize from the file
void VehicleRepoVector::init( const std::vector<Vehicle> &vehicleSet )
{
   std::chrono::system_clock::time_point before =
         std::chrono::system_clock::now();

   m_repoVector = vehicleSet;

   // in-place sort
   std::sort( m_repoVector.begin(), m_repoVector.end() );

   // eliminate dups ( needs to be after sort since unique only eliminates dups that are sequential )
   size_t beforeSize = m_repoVector.size();
   m_repoVector.erase( unique( m_repoVector.begin(), m_repoVector.end() ),
                       m_repoVector.end() );

   std::chrono::system_clock::time_point after =
         std::chrono::system_clock::now();

   m_initTime = (std::chrono::duration_cast<std::chrono::microseconds>)(
         after - before ).count();

   // print out how many dups were found.
   size_t afterSize = m_repoVector.size();
   size_t numDups = beforeSize - afterSize;

   cout << "\nVehicleRepoVector::" << __FUNCTION__ << "\n size():" << afterSize
         << " duplicates:" << numDups << "\n insertions/sorting/dups took "
         << m_initTime << "us." << endl;
   audit();  // do not include audit in the time.
}

void VehicleRepoVector::print( ostream &ios ) const
{
   if( m_verbose )
   {
      ios
            << "..........printing entire repository  for VehicleRepoVector ..............."
            << endl;

      for( const auto &item : m_repoVector )
      {
         if( m_verbose ) item.printAsString( cout );
      }
   }
   ios << "m_repoVector has " << m_repoVector.size() << " items" << endl;
}

void VehicleRepoVector::printRandomVehicleVin() const
{
   uint32_t idx = rand() % m_repoVector.size();
   std::string vin = m_repoVector[idx].getVin();
   std::cout << vin << std::endl;
}

size_t VehicleRepoVector::searchByYear( uint16_t year ) const
{
   std::vector<Vehicle> s_searchByYear;

   // linearly search the vector and if a match occurs make a copy and put it in another vector s_searchByYear.
   std::copy_if( m_repoVector.begin(), m_repoVector.end(),
                 std::back_inserter( s_searchByYear ), [ year ]( Vehicle i )
                 {  return i.getYear() == year;} );

   if( m_verbose )
   {
      for( const auto &item : s_searchByYear )
      {
         assert( item.getYear() == year );
         item.printAsString( cout );  // print test results.
      }
   }
   return s_searchByYear.size();
}

size_t VehicleRepoVector::searchByModel( Vehicle::e_Model model ) const
{

   std::vector<Vehicle> s_searchByModel;
   // search the vector and if a match occurs, copy into s_searchByModel vector
   std::copy_if( m_repoVector.begin(), m_repoVector.end(),
                 std::back_inserter( s_searchByModel ), [ model ]( Vehicle i )
                 {  return (i.getModel() == model);} );

   if( m_verbose )
   {
      for( const auto &item : s_searchByModel )
      {
         assert( item.getModel() == model );
         item.printAsString( cout );  // print test results.
      }
   }
   return s_searchByModel.size();
}

bool VehicleRepoVector::searchByVin( const std::string &vin ) const
{
   // search this->m_repoVector for vehicle; use dummy fields.
   Vehicle v( vin, Vehicle::e_lastModel, Vehicle::e_lastColor, 2005 );

   auto it = std::lower_bound( m_repoVector.begin(), m_repoVector.end(), v,
                               []( const Vehicle &l, const Vehicle &r )
                               {  return l < r;} );
   if( !(it != m_repoVector.end() && *it == v) ) return false;

   it->printAsString( cout );
   return true;
}

void VehicleRepoVector::traverse() const
{
   for( const auto &element : m_repoVector )
   {
      element.getScratch();
   }
}

void VehicleRepoVector::traverseAndWrite()
{
   for( auto &element : m_repoVector )
   {
      element.incrementScratch();
   }
}

