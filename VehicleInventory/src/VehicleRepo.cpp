/*
 * VehicleRepo.cpp
 *
 *  Created on: Nov 20, 2019
 *      Author: laurence
 */

#include <cassert>
#include "../inc/VehicleRepo.hpp"

size_t VehicleRepo::countTotalByYear() const
{
   size_t yearCount = 0;
   for( uint16_t year = Vehicle::LowestYear; year < Vehicle::HighestYear;
         year++ )
   {
      yearCount = yearCount + this->searchByYear( year );
   }
   return yearCount;
}

size_t VehicleRepo::countTotalByModel() const
{
   size_t modelCount = 0;
   for( uint16_t model = Vehicle::e_Mustang; model < Vehicle::e_lastModel;
         model++ )
   {
      std::stringstream ss;
      modelCount = modelCount + this->searchByModel( (Vehicle::e_Model) model );
   }
   return modelCount;
}

void VehicleRepo::audit() const
{
   assert( countTotalByModel() == countTotalByVin() );
   assert( countTotalByYear()  == countTotalByVin() );
   return;
}

