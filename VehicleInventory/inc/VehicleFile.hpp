/*
 * VehicleFile.hpp
 *
 *  Created on: Nov 4, 2019
 *      Author: laurence
 */

#ifndef VEHICLEFILE_HPP_
#define VEHICLEFILE_HPP_

#include "Vehicle.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <random>
#include <vector>

class VehicleFile
{
public:
  VehicleFile( const std::string& fileName, bool verbose ) :
      m_fileName( fileName ),
      m_verbose( verbose ),
      m_gen( std::random_device{}() )
      {}
  virtual ~VehicleFile() = default;
  void createVehicleFile(size_t number, ostream& ios );
  inline std::vector<Vehicle>& getVehicleVector() { return m_vehicle; }
  void print( ostream& ios ) const;
  static size_t countLines( istream& ios );
private:
  VehicleFile() = default;
  VehicleFile( const VehicleFile& v ) = default;
  Vehicle::e_Model generateModel() const;
  void generateVin( std::string& vin );
  uint16_t generateYear();
  Vehicle::e_Color generateColor() const;

  // members
  std::vector<Vehicle> m_vehicle;
  std::string m_fileName;
  bool m_verbose = false;
  boost::random::mt19937 m_gen;
};

#endif /* VEHICLEFILE_HPP_ */
