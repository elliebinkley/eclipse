/*
 * VehicleRepo_Instrusive.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: laurence
 */

#include <boost/functional/hash.hpp>
#include <chrono>
#include <cstdint>
#include <array>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>

#include "../inc/VehicleFile.hpp"
#include "../inc/VehicleRepoIntrusive.hpp"

VehicleRepoIntrusive::~VehicleRepoIntrusive()
{
   cleanup();
   if( m_vPtr ) free( m_vPtr );
}

void VehicleRepoIntrusive::init( const std::vector<Vehicle> &vehicleSet )
{
   // get enough memory for all the Vehicles.
   std::chrono::system_clock::time_point before =
         std::chrono::system_clock::now();
   m_vPtr = (VehicleIntrusive*) malloc(
         sizeof(VehicleIntrusive) * vehicleSet.size() );
   assert( m_vPtr );

   uint32_t localCount = 0;
   uint32_t numDups = 0;
   VehicleIntrusive *iter_vPtr = m_vPtr;
   for( const auto &vehicle : vehicleSet )
   {
      try
      {
         //  Initialize Vehicle from memory previous malloc(ed)
         VehicleIntrusive *vehicleIntrusive =
               new ( iter_vPtr++ ) VehicleIntrusive( vehicle );

         // insert into the repository set key by VIN;
         auto ret = m_repoIntrusiveByVin.insert( *vehicleIntrusive );
         if( !ret.second )
         {
            // probably a dup failure; simply reject dups of VINS.
            numDups++;
            continue;
         }
         auto ret1 = m_repoIntrusiveByYear.insert( *vehicleIntrusive );
         if( (ret1->getVin() != vehicleIntrusive->getVin()) )
         {
            cout << "m_repoIntrusiveByYear Vin mismatch for " << endl;
            vehicleIntrusive->printAsString( cout );
            ret1->printAsString( cout );
         }
         auto ret2 = m_repoIntrusiveByModel.insert( *vehicleIntrusive );
         if( (ret2->getVin() != vehicleIntrusive->getVin()) )
         {
            cout << "m_repoIntrusiveByModel Vin mismatch for " << endl;
            vehicleIntrusive->printAsString( cout );
            ret2->printAsString( cout );
         }

      }
      catch( const char *e )
      {
         cerr << e << endl;
      }
      localCount++;
   }

   std::chrono::system_clock::time_point after =
         std::chrono::system_clock::now();
   m_initTime = std::chrono::duration_cast<std::chrono::microseconds>(
         after - before ).count();

   cout << "\n VehicleRepoIntrusive::" << __FUNCTION__ << "\n insertions:"
         << localCount << " duplicates:" << numDups << "\n took " << m_initTime
         << "us." << endl;

   audit();  // do not include audit in the elapsed time.
}

void VehicleRepoIntrusive::print( ostream &ios ) const
{
   ios << "m_repoIntrusiveByVin.size():" << m_repoIntrusiveByVin.size()
         << std::endl;
   if( m_verbose )
   {
      ios
            << "..........printing entire repository for m_repoIntrusiveByVin ......."
            << std::endl;
      for( auto it = this->m_repoIntrusiveByVin.begin();
            it != this->m_repoIntrusiveByVin.end(); it++ )
      {
         it->printAsString( cout );
      }
   }

   ios << "m_repoIntrusiveByYear size " << m_repoIntrusiveByYear.size()
         << std::endl;
   if( m_verbose )
   {
      ios
            << "..........printing entire repository for m_repoIntrusiveByYear........"
            << std::endl;
      for( auto it = this->m_repoIntrusiveByYear.begin();
            it != this->m_repoIntrusiveByYear.end(); it++ )
      {
         it->printAsString( cout );
      }
   }

   ios << "m_repoIntrusiveByModel size " << m_repoIntrusiveByModel.size()
         << std::endl;
   if( m_verbose )
   {
      ios
            << "..........printing entire repository for m_repoIntrusiveByModel........"
            << std::endl;
      for( auto it = this->m_repoIntrusiveByModel.begin();
            it != this->m_repoIntrusiveByModel.end(); it++ )
      {
         it->printAsString( cout );
      }
   }
}

size_t VehicleRepoIntrusive::searchByModel( Vehicle::e_Model model ) const
{
   auto range = m_repoIntrusiveByModel.equal_range( model );
   if( range.first == m_repoIntrusiveByModel.end() ) return 0;
   if( m_verbose )
   {
      for( auto it = range.first; it != range.second; it++ )
      {
         assert( it->getModel() == model );
         it->printAsString( cout );
      }
   }
   size_t numFound = std::distance( range.first, range.second );
   return numFound;
}

size_t VehicleRepoIntrusive::searchByYear( uint16_t year ) const
{
   auto range = m_repoIntrusiveByYear.equal_range( year );
   if( range.first == m_repoIntrusiveByYear.end() ) return 0;

   if( m_verbose )
   {
      for( auto it = range.first; it != range.second; it++ )
      {
         assert( it->getYear() == year );
         it->printAsString( cout );
      }
   }
   size_t numFound = std::distance( range.first, range.second );
   return numFound;
}

bool VehicleRepoIntrusive::searchByVin( const std::string &vin ) const
{
   auto element = m_repoIntrusiveByVin.find( vin );

   if( element == m_repoIntrusiveByVin.end() ) return false;
   if( element->getVin() != vin ) return false;

   element->printAsString( cout );
   return true;
}

void VehicleRepoIntrusive::cleanup()
{
   if( !m_repoIntrusiveByYear.empty() )
   {
      cout << "cleaning up m_repoIntrusiveByYear:"
            << m_repoIntrusiveByYear.size() << endl;
      m_repoIntrusiveByYear.erase( m_repoIntrusiveByYear.begin(),
                                   m_repoIntrusiveByYear.end() );
      assert( m_repoIntrusiveByYear.empty() );
   }
   if( !m_repoIntrusiveByModel.empty() )
   {
      cout << "cleaning up m_repoIntrusiveByModel:"
            << m_repoIntrusiveByModel.size() << endl;
      m_repoIntrusiveByModel.erase( m_repoIntrusiveByModel.begin(),
                                    m_repoIntrusiveByModel.end() );
      assert( m_repoIntrusiveByModel.empty() );
   }

   // erase and call the DTOR for each element
   if( !m_repoIntrusiveByVin.empty() )
   {
      cout << "cleaning up and depose m_repoIntrusiveByVin" << endl;
      m_repoIntrusiveByVin.erase_and_dispose( m_repoIntrusiveByVin.begin(),
                                              m_repoIntrusiveByVin.end(),
                                              disposer() );
      assert( m_repoIntrusiveByVin.empty() );
   }
}

void VehicleRepoIntrusive::traverse() const
{
   for( const auto &element : m_repoIntrusiveByVin )
   {
      element.getScratch();
   }
}

void VehicleRepoIntrusive::traverseAndWrite()
{
   for( auto &element : m_repoIntrusiveByVin )
   {
      element.incrementScratch();
   }
}

