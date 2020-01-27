/*
 * Vehicle.hpp
 *
 *  Created on: Oct 25, 2019
 *      Author: laurence
 */

#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

class Vehicle
{
public:

   enum e_Manufacturer
   {
      e_FORD, e_BMW, e_GM, e_Toyota, e_Tesla, e_lastManufacturer
   };

   enum e_Model
   {
      e_Mustang, e_f150, e_i3, e_i8, e_x7, e_z4, e_camaro, e_ModelS,
      e_Model3, e_Corolla, e_Camry, e_lastModel
   };

   enum e_Color
   {
      e_Red, e_Blue, e_Green, e_Yellow, e_Black, e_White, e_lastColor
   };

   const typedef struct
   {
      e_Manufacturer index;
      const char *name;
      const char *info;
   } s_ManufacturerInfo;

   const typedef struct
   {
      e_Color index;
      const char *name;
   } s_Color;

   const typedef struct
   {
      e_Model index;
      e_Manufacturer indexManuf;
      const char *name;
   } s_Model;

   // normally these constexpr items would be in a database .....
   static constexpr std::array<Vehicle::s_Color, Vehicle::e_Color::e_lastColor> m_colorMap =
     { {
     { Vehicle::e_Color::e_Red, "red" },
     { Vehicle::e_Color::e_Blue, "blue" },
     { Vehicle::e_Color::e_Green, "green" },
     { Vehicle::e_Color::e_Yellow, "yellow" },
     { Vehicle::e_Color::e_Black, "black" },
     { Vehicle::e_Color::e_White, "white" }
     } };

   static constexpr std::array<Vehicle::s_ManufacturerInfo,
     Vehicle::e_Manufacturer::e_lastManufacturer> m_manufacturerInfo =
     { {
     { Vehicle::e_Manufacturer::e_FORD, "Ford", "" },
     { Vehicle::e_Manufacturer::e_BMW, "BMW", "© BMW AG, Munich, Germany" },
     { Vehicle::e_Manufacturer::e_GM, "General Motors", "" },
     { Vehicle::e_Manufacturer::e_Toyota, "Toyota", "" },
     { Vehicle::e_Manufacturer::e_Tesla, "Tesla", "(Batteries Included!) " }
     } };

   static constexpr std::array<Vehicle::s_Model, Vehicle::e_lastModel> m_modelMap =
     { {
     { Vehicle::e_Model::e_Mustang,Vehicle::e_Manufacturer::e_FORD,"mustang" },
     { Vehicle::e_Model::e_f150,Vehicle::e_Manufacturer::e_FORD, "F150" },
     { Vehicle::e_Model::e_i3, Vehicle::e_Manufacturer::e_BMW, "i3" },
     { Vehicle::e_Model::e_i8, Vehicle::e_Manufacturer::e_BMW, "i8" },
     { Vehicle::e_Model::e_x7, Vehicle::e_Manufacturer::e_BMW, "x7" },
     { Vehicle::e_Model::e_z4, Vehicle::e_Manufacturer::e_BMW, "z4" },
     { Vehicle::e_Model::e_camaro, Vehicle::e_Manufacturer::e_GM, "camaro" },
     { Vehicle::e_Model::e_ModelS, Vehicle::e_Manufacturer::e_Tesla, "Model S" },
     { Vehicle::e_Model::e_Model3, Vehicle::e_Manufacturer::e_Tesla, "Model 3" },
     { Vehicle::e_Model::e_Corolla, Vehicle::e_Manufacturer::e_Toyota, "Corolla" },
     { Vehicle::e_Model::e_Camry, Vehicle::e_Manufacturer::e_Toyota, "Camry" }
     } };

   static constexpr uint16_t LowestYear = 1990;
   static constexpr uint16_t HighestYear = 2022;
   static constexpr uint16_t lowYear = 1990;
   static constexpr uint16_t highYear = 2019;

   // Ctor
   Vehicle( const std::string& vin, const e_Model model, const e_Color color, uint16_t year ) :
      m_vin( vin ),
      m_model( model ),
      m_color( color ),
      m_year( year ),
      m_scratch( 0 )
   {
      m_vehicleCtorCnt++;
   }

   //Ctor       line is space separated vin modelIndex ColorIndex year
   Vehicle( const std::string line );

   // Copy constructor
   Vehicle( const Vehicle& v ) :
      m_vin( v.m_vin ),
      m_model( v.m_model ),
      m_color( v.m_color ),
      m_year( v.m_year ),
      m_scratch( v.m_scratch )
   {
      m_vehicleCpCtorCnt++;
   }

   // Move CTOR
   //    C++ 11
   //    Note: all STL containers support std::move.
   Vehicle( Vehicle&& v ) noexcept :
         m_vin( std::move( v.m_vin )),
         m_model( std::move( v.m_model )),
         m_color( std::move( v.m_color )),
         m_year ( std::move( v.m_year )),
         m_scratch( std::move( v.m_scratch ))
   {
      m_vehicleMoveCtorCnt++;
   }

   // copy assignment operator
   //    C++ 11
   Vehicle& operator=( const Vehicle& other )
   {
      if( this == &other ) return( *this );
      this->m_vin = other.m_vin;
      this->m_model = other.m_model;
      this->m_color = other.m_color;
      this->m_year = other.m_year;
      this->m_scratch = other.m_scratch;
      m_vehicleAssignCtorCnt++;
      return( *this );
   }

   virtual ~Vehicle()
   {
      m_vehicleDtorCnt++;
   }

   // methods
   static void clearCounts();
   std::string getInfo() const;
   inline uint16_t getYear() const { return m_year; }
   inline e_Color getColor() const { return m_color;}
   std::string getColorAsString() const;
   e_Manufacturer getManufacturer() const;
   std::string getManufacturerAsString() const;
   std::string getManufacturerInfo() const;
   inline e_Model getModel() const { return m_model;}
   inline std::string getModelAsString() const;
   inline uint16_t getScratch() const { return m_scratch; }
   inline std::string getVin() const { return m_vin;}
   inline void incrementScratch() { m_scratch++; }
   static bool isNumber( const string& str );
   static bool isValidYear( const string& str, uint16_t *year );
   virtual void printAsString( std::ostream& ios ) const;
   virtual void print( std::ostream& ios ) const;
   static void printCounts( std::ostream& ios );

   // relational operators
   inline bool operator == (const Vehicle& l)  { return( m_vin == l.m_vin ); }
   inline bool operator <  (const Vehicle& l)  { return( m_vin < l.m_vin );  }
   inline bool operator <= (const Vehicle& l)  { return( m_vin <= l.m_vin ); }
   inline bool operator >  (const Vehicle& l)  { return( m_vin > l.m_vin );  }
   inline bool operator >= (const Vehicle& l)  { return( m_vin >= l.m_vin ); }
   inline bool operator != (const Vehicle& l)  { return( m_vin != l.m_vin ); }

   friend bool operator == (const Vehicle& lhs, const Vehicle& rhs)
      { return( lhs.m_vin == rhs.m_vin ); }
   friend bool operator <  (const Vehicle& lhs, const Vehicle& rhs)
      { return( lhs.m_vin < rhs.m_vin );  }
   friend bool operator <= (const Vehicle& lhs, const Vehicle& rhs)
      { return( lhs.m_vin <= rhs.m_vin ); }
   friend bool operator >= (const Vehicle& lhs, const Vehicle& rhs)
      { return( lhs.m_vin >= rhs.m_vin ); }
   friend bool operator >  (const Vehicle& lhs, const Vehicle& rhs)
      { return( lhs.m_vin > rhs.m_vin );  }
   friend bool operator != (const Vehicle& lhs, const Vehicle& rhs)
      { return( lhs.m_vin != rhs.m_vin ); }
   friend std::ostream& operator << (std::ostream& os, const Vehicle& vehicle)
   {
      vehicle.print (os);
      return os;
   }


   static uint32_t m_vehicleCtorCnt;
   static uint32_t m_vehicleLineCtorCnt;
   static uint32_t m_vehicleMoveCtorCnt;
   static uint32_t m_vehicleCpCtorCnt;
   static uint32_t m_vehicleAssignCtorCnt;
   static uint32_t m_vehicleDtorCnt;
   static bool m_verbose;

private:

   Vehicle();               // private constructor
   std::string m_vin;
   e_Model m_model;
   e_Color m_color;
   uint16_t m_year;
   uint16_t m_scratch;

};

#endif /* VEHICLE_HPP_ */
