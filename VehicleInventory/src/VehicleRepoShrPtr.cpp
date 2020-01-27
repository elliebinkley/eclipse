/*
 * Vehicle.cpp
 *
 *  Created on: Oct 21, 2019
 *      Author: laurence
 */


#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

#include "../inc/VehicleFile.hpp"
#include "../inc/VehicleRepoShrPtr.hpp"

void VehicleRepoShrPtr::init( const std::vector<Vehicle> &vehicleSet )
{
   // insert vehicles into data structures
   std::chrono::system_clock::time_point before =
         std::chrono::system_clock::now();
   size_t localCount = 0;
   uint32_t numDups = 0;
   for( const auto &vehicle : vehicleSet )
   {
      try
      {
         // make a pair (vin,shared_ptr to vehicle)
         // note: make a copy of the vehicles in vehicleSetnnnnnnnnnnnnnnnnnnnnnn
         std::pair<std::string, std::shared_ptr<Vehicle>> pair(
               vehicle.getVin(),
               std::shared_ptr<Vehicle>( new Vehicle( vehicle ) ) );

         // insert pair into map keyed by VIN
         auto ret = m_repoShrPtrByVin.insert( pair );
         if( !ret.second )
         {
            numDups++;
            continue;
         }

         // insert into multimap keyed by year
         uint16_t year = pair.second->getYear();
         std::pair<uint16_t, std::shared_ptr<Vehicle> > pairYear =
               std::make_pair( year, std::move( pair.second ) );
         m_repoShrPtrByYear.insert( pairYear );

         // insert into multimap keyed by model
         Vehicle::e_Model model = pairYear.second->getModel();
         std::pair<Vehicle::e_Model, std::shared_ptr<Vehicle> > pairModel =
               std::make_pair( model, std::move( pairYear.second ) );
         m_repoShrPtrByModel.insert( pairModel );
      }
      catch( std::string &e )
      {
         std::cerr << "catch():" << e << std::endl;
      }
      catch( std::exception &e )
      {
         std::cerr << "catch():" << e.what() << std::endl;
      }
      catch( ... )
      {
         std::cerr << "catch():" << std::endl;
      }
      localCount++;
   }

   std::chrono::system_clock::time_point after =
         std::chrono::system_clock::now();
   m_initTime = (std::chrono::duration_cast<std::chrono::microseconds>)(
         after - before ).count();
   cout << "\nVehicleRepoShrPtr::" << __FUNCTION__ << "\n insertions:"
         << localCount << " duplicates:" << numDups << "\n took " << m_initTime
         << "us." << endl;

   cout << "m_repoShrPtrByModel.size():" << m_repoShrPtrByModel.size() << endl;
   cout << "m_repoShrPtrByYear.size():" << m_repoShrPtrByYear.size() << endl;
   cout << "m_repoShrPtrByVin.size():" << m_repoShrPtrByVin.size() << endl;

   audit();  // do not include audit in the time.
}

bool VehicleRepoShrPtr::remove( std::string &vin )
{
   // find the element
   std::map<std::string, std::shared_ptr<Vehicle>>::iterator it;
   it = m_repoShrPtrByVin.find( vin );
   if( it == m_repoShrPtrByVin.end() )
   {
      return false;  // not found
   }

   // delete from year map
   uint16_t year = it->second->getYear();

   {
      auto range = m_repoShrPtrByYear.equal_range( year );
      for( auto it1 = range.first; it1 != range.second; it1++ )
      {
         if( it1->second.get()->getVin() == vin )
            m_repoShrPtrByYear.erase( it1 );
      }
   }

   // delete from model map
   Vehicle::e_Model model = it->second->getModel();
   {
      auto range = m_repoShrPtrByModel.equal_range( model );
      for( auto it1 = range.first; it1 != range.second; it1++ )
      {
         if( it1->second.get()->getVin() == vin )
            m_repoShrPtrByModel.erase( it1 );
      }
   }
   // delete from VIN map
   m_repoShrPtrByVin.erase( it );
   return true;
}

bool VehicleRepoShrPtr::add( const std::string &vin,
                             const Vehicle::e_Manufacturer manufacturer,
                             const Vehicle::e_Model model,
                             const Vehicle::e_Color color, uint16_t year )
{
   return add( Vehicle( vin, model, color, year ) );
}

bool VehicleRepoShrPtr::add( Vehicle vehicle ) // make a copy? TODO see if reference will work.
{
   bool retVal = false;
   // Forces map key to have the same key (VIN) as in vehicle class. ( no dups allowed)
   std::shared_ptr<Vehicle> ptr = make_shared<Vehicle>( vehicle );
   auto pair = std::make_pair( ptr->getVin(), std::move( ptr ) );
   std::pair<std::map<std::string, std::shared_ptr<Vehicle>>::iterator, bool> ret;
   ret = m_repoShrPtrByVin.insert( pair );
   if( ret.second )
   {
      cout << " insert success for VIN=" << ret.first->second.get()->getVin()
            << endl;
   }
   else
   {
      cout << "insert failed ( possible duplicate..) for VIN="
            << vehicle.getVin() << endl;
      return false;
   }

   // insert to the year map
   std::shared_ptr<Vehicle> ptrYear = make_shared<Vehicle>( vehicle );
   auto pairYear = std::make_pair( ptrYear->getYear(), std::move( ptrYear ) );
   m_repoShrPtrByYear.insert( pairYear );

   // insert to the model map
   std::shared_ptr<Vehicle> ptrModel = make_shared<Vehicle>( vehicle );
   auto pairModel = std::make_pair( ptrModel->getModel(),
                                    std::move( ptrModel ) );
   m_repoShrPtrByModel.insert( pairModel );

   return retVal;
}

void VehicleRepoShrPtr::print( ostream &ios ) const
{
   if( m_verbose )
   {
      ios
            << "..........printing entire repository  for VehicleRepoShrPtr ..............."
            << endl;
      ios << "..........printing by VIN ..............." << endl;
      for( const auto &car : m_repoShrPtrByVin )
      {
         car.second.get()->printAsString( ios );
      }
      ios << "..........printing by YEAR..............." << endl;
      for( const auto &car : m_repoShrPtrByYear )
      {
         car.second.get()->printAsString( ios );
      }
      ios << "..........printing by Model..............." << endl;
      for( const auto &car : m_repoShrPtrByModel )
      {
         car.second.get()->printAsString( ios );
      }
      assert( m_repoShrPtrByVin.size() == m_repoShrPtrByYear.size() );
      assert( m_repoShrPtrByVin.size() == m_repoShrPtrByModel.size() );
   }
   ios << "m_repoShrPtrByVin/Model/Year has " << m_repoShrPtrByVin.size()
         << " items" << endl;
}

size_t VehicleRepoShrPtr::searchByModel( Vehicle::e_Model model ) const
{
   auto range = m_repoShrPtrByModel.equal_range( model );
   if( range.first == m_repoShrPtrByModel.end() ) return 0;

   if( m_verbose )
   {
      for( auto it = range.first; it != range.second; it++ )
      {
         assert( it->second->getModel() == model );
         it->second->printAsString( cout );
      }
   }
   size_t numFound = std::distance( range.first, range.second );
   return numFound;
}

size_t VehicleRepoShrPtr::searchByYear( uint16_t year ) const
{
   auto range = m_repoShrPtrByYear.equal_range( year );
   if( range.first == m_repoShrPtrByYear.end() ) return 0;

   if( m_verbose )
   {
      for( auto it = range.first; it != range.second; it++ )
      {
         assert( it->second->getYear() == year );
         it->second->printAsString( cout );
      }
   }

   size_t numFound = std::distance( range.first, range.second );
   return numFound;
}

bool VehicleRepoShrPtr::searchByVin( const std::string &vin ) const
{
   auto element = m_repoShrPtrByVin.find( vin );

   if( element == m_repoShrPtrByVin.end() ) return false;
   if( element->second->getVin() != vin ) return false;

   element->second->printAsString( cout );
   return true;
}

void VehicleRepoShrPtr::cleanup()
{
   // delete shared pointers/elements ; last share pointer should delete object.

   cout << "cleaning up m_repoShrPtrByVin" << endl;
   m_repoShrPtrByVin.erase( m_repoShrPtrByVin.begin(),
                            m_repoShrPtrByVin.end() );
   assert( m_repoShrPtrByVin.empty() );

   // TODO validate that a bucket erase frees the elements in the bucket.
   cout << "cleaning up m_repoShrPtrByYear" << endl;
   m_repoShrPtrByYear.erase( m_repoShrPtrByYear.begin(),
                             m_repoShrPtrByYear.end() );
   assert( m_repoShrPtrByYear.empty() );

   cout << "cleaning up m_repoShrPtrByModel" << endl;
   m_repoShrPtrByModel.erase( m_repoShrPtrByModel.begin(),
                              m_repoShrPtrByModel.end() );
   assert( m_repoShrPtrByModel.empty() );
}

void VehicleRepoShrPtr::traverse() const
{
   for( const auto &element : m_repoShrPtrByVin )
   {
      element.second->getScratch();
   }
}

void VehicleRepoShrPtr::traverseAndWrite()
{
   for( auto &element : m_repoShrPtrByVin )
   {
      element.second->incrementScratch();
   }
}

