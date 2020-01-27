/*
 * Vehicle.cpp
 *
 *  Created on: Oct 25, 2019
 *      Author: laurence
 */

#include <sstream>
#include <iomanip>
#include <cstdio>
#include "../inc/Vehicle.hpp"

uint32_t Vehicle::m_vehicleCtorCnt = 0;
uint32_t Vehicle::m_vehicleLineCtorCnt = 0;
uint32_t Vehicle::m_vehicleMoveCtorCnt = 0;
uint32_t Vehicle::m_vehicleCpCtorCnt = 0;
uint32_t Vehicle::m_vehicleAssignCtorCnt = 0;
uint32_t Vehicle::m_vehicleDtorCnt = 0;

bool m_verbose = false;
bool Vehicle::m_verbose = false;

//CTOR
Vehicle::Vehicle( const std::string line )
{
   std::istringstream ss( line );

   // 1st field is VIN
   std::getline( ss, m_vin, ' ' );
   if( m_vin == "" )
   {
      throw std::string( "invalid vin in line:" + line );
   }

   // second field is model
   std::string tmp;
   std::getline( ss, tmp, ' ' );
   if( !isNumber( tmp ) )
   {
      throw std::string( "invalid model index in line:" + line );
   }
   m_model = (Vehicle::e_Model) std::atoi( tmp.c_str() );
   if( m_model > Vehicle::e_Model::e_lastModel )
   {
      throw std::string( "invalid model index in line:" + line );
   }

   // third field is color
   std::getline( ss, tmp, ' ' );
   if( !isNumber( tmp ) )
   {
      throw std::string( "invalid color index in line:" + line );
   }

   m_color = (Vehicle::e_Color) std::atoi( tmp.c_str() );
   if( m_color > Vehicle::e_Color::e_lastColor )
   {
      throw std::string( "invalid color index in line:" + line );
   }

   // fourth field is year
   std::getline( ss, tmp, ' ' );
   m_year = std::atoi( tmp.c_str() );
   if( m_year < Vehicle::LowestYear || m_year > (Vehicle::HighestYear) )
   {
      throw std::string( "invalid year in line:" + line );
   }
   m_scratch = 0;
   m_vehicleLineCtorCnt++;
}

void Vehicle::print( std::ostream &ios ) const
{
   std::stringstream ss;
   ss << getVin() << " " << getModel() << " " << getColor() << " " << getYear()
         << std::endl;
   ios << ss.str();
}

void Vehicle::printAsString( std::ostream &ios ) const
{
   ios << getInfo();
}

std::string Vehicle::getManufacturerAsString() const
{
   std::string s1( m_manufacturerInfo[getManufacturer()].name );
   return s1;
}

std::string Vehicle::getModelAsString() const
{
   std::string s( m_modelMap[getModel()].name );
   return s;
}

std::string Vehicle::getColorAsString() const
{
   return Vehicle::m_colorMap[this->m_color].name;
}

bool Vehicle::isNumber( const string &str )
{
   string::const_iterator itr = str.begin();
   while( itr != str.end() && isdigit( *itr ) )
      ++itr;
   if( !str.empty() && itr == str.end() )
   {
      return true;
   }
   return false;
}

bool Vehicle::isValidYear( const string &str, uint16_t *year )
{
   if( !isNumber( str ) )
   {
      return false;
   }
   *year = atoi( str.c_str() );
   if( *year < Vehicle::LowestYear || *year > Vehicle::HighestYear )
      return false;
   return true;
}

Vehicle::e_Manufacturer Vehicle::getManufacturer() const
{
   return (m_modelMap[m_model].indexManuf);
}

std::string Vehicle::getManufacturerInfo() const
{
   std::string s1( m_manufacturerInfo[getManufacturer()].info );
   return s1;
}

std::string Vehicle::getInfo() const
{
   std::stringstream ss = std::stringstream();
   ss << "Model:" << std::setw( 15 ) << left << m_modelMap[m_model].name
         << "  Year:" << std::setw( 5 ) << left << getYear() << "  Vin:"
         << std::setw( 15 ) << left << m_vin << "  color:" << std::setw( 8 )
         << left << m_colorMap[getColor()].name << "  Manufacturer:"
         << m_manufacturerInfo[getManufacturer()].name << " "
         << getManufacturerInfo() << "  Address:" << (void*) this << std::endl;
   return ss.str();
}

void Vehicle::clearCounts()
{
   m_vehicleCtorCnt = 0;
   m_vehicleLineCtorCnt = 0;
   m_vehicleMoveCtorCnt = 0;
   m_vehicleCpCtorCnt = 0;
   m_vehicleAssignCtorCnt = 0;
   m_vehicleDtorCnt = 0;
}

void Vehicle::printCounts( std::ostream &ios )
{
   ios << "m_vehicleCtorCnt=" << m_vehicleCtorCnt << " m_vehicleLineCtorCnt="
         << m_vehicleLineCtorCnt << " m_vehicleMoveCtorCnt="
         << m_vehicleMoveCtorCnt << " m_vehicleCpCtorCnt=" << m_vehicleCpCtorCnt
         << " m_vehicleAssignCtorCnt=" << m_vehicleAssignCtorCnt
         << " m_vehicleDtorCnt=" << m_vehicleDtorCnt << std::endl;
}
