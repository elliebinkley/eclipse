/*
 * VehicleFile.cpp
 *
 *  Created on: Nov 4, 2019
 *      Author: laurence
 */

#include "../inc/VehicleFile.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <random>

// create a file with vehicles
void VehicleFile::createVehicleFile( size_t number, ostream &ios )
{
   // make vehicle repository in m_fileName
   if( m_fileName == "" )
   {
      cerr << "Empty filename; createVehicleFile failed" << endl;
      return;
   }
   std::ofstream ofile;
   ofile.open( m_fileName );

   // add a known list of vehicles
   std::vector<Vehicle> vehicleVector
   { {
   { Vehicle( "VIN0000001", Vehicle::e_Model::e_f150,   Vehicle::e_Color::e_Black,  2005 ) },
   { Vehicle( "VIN0000002", Vehicle::e_Model::e_i3,     Vehicle::e_Color::e_White,  2010 ) },
   { Vehicle( "VIN0000003", Vehicle::e_Model::e_Camry,  Vehicle::e_Color::e_Yellow, 2010 ) },
   { Vehicle( "VIN0000004", Vehicle::e_Model::e_ModelS, Vehicle::e_Color::e_Red,    2018 ) }
   } };

   for( const Vehicle &v : vehicleVector )
   {
      v.print( ofile );
      m_vehicle.push_back( v );
   }

   // add more known vehicles
   Vehicle v1( "VIN0000005", Vehicle::e_Model3, Vehicle::e_Color::e_Black, 2019 );
   v1.print( ofile );
   m_vehicle.push_back( v1 );

   Vehicle v2( "VIN0000006", Vehicle::e_Model::e_Mustang, Vehicle::e_Color::e_Black, 2005 );
   v2.print( ofile );
   m_vehicle.push_back( v2 );

   // add a number of random vehicles
   srand( time( NULL ) );
   for( size_t i = 0; i < number; i++ )
   {
      std::string vin;
      generateVin( vin );
      auto model = generateModel();
      auto year = generateYear();
      auto color = generateColor();
      try
      {
         Vehicle vehicle( vin, model, color, year );
         vehicle.print( ofile );
         m_vehicle.push_back( vehicle );
      }
      catch( std::string &e )
      {
         std::cerr << "catch():" << e << std::endl;
      }
      catch( ... )
      {
         std::cerr << "catch():" << std::endl;
      }
   }
   ofile.close();
   ios << "created " << (number + 6) << " vehicles in file:" << m_fileName
         << std::endl;
}

Vehicle::e_Color VehicleFile::generateColor() const
{
   return ((Vehicle::e_Color) (rand() % Vehicle::e_Color::e_lastColor));
}

void VehicleFile::generateVin( std::string &vin )
{
   std::stringstream ss;
   boost::random::uniform_int_distribution<> dist( 000001000, 2000000000 );
   ss << "VIN" << dist( m_gen );
   vin = ss.str();
   return;
}

Vehicle::e_Model VehicleFile::generateModel() const
{
   return ((Vehicle::e_Model) (rand() % Vehicle::e_Model::e_lastModel));
}

uint16_t VehicleFile::generateYear()
{
   return ((rand() % (Vehicle::HighestYear - Vehicle::LowestYear))
         + Vehicle::LowestYear);
}

// count lines in a file by counting the number of '\n' in the istream;
size_t VehicleFile::countLines( istream &ios )
{
   // skip when bad
   if( ios.bad() ) return 0;
   // save state
   std::istream::iostate state_backup = ios.rdstate();
   // clear state
   ios.clear();
   // save postion in the file
   std::istream::streampos pos_backup = ios.tellg();

   // go to the beginning of the file and count the line feeds
   ios.seekg( 0 );
   size_t line_cnt;
   size_t lf_cnt = std::count( std::istreambuf_iterator<char>( ios ),
                               std::istreambuf_iterator<char>(), '\n' );
   line_cnt = lf_cnt;

   // if the file is not end with '\n' , then line_cnt should plus 1
   ios.unget();
   if( ios.get() != '\n' )
   {
      ++line_cnt;
   }

   // recover state
   ios.clear(); // previous reading may set eofbit
   ios.seekg( pos_backup );
   ios.setstate( state_backup );

   return line_cnt;
}

