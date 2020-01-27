/*
 * VehicleRepo.hpp
 *
 *  Created on: Nov 4, 2019
 *      Author: laurence
 */

#ifndef VEHICLEREPO_HPP_
#define VEHICLEREPO_HPP_


#include <chrono>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Vehicle.hpp"

// abstract class for every repository
class VehicleRepo
{
public:
  VehicleRepo( bool verbose ) : m_verbose( verbose ) { }
  virtual ~VehicleRepo() = default;

  void audit() const;
  virtual void cleanup() = 0;
  virtual size_t countTotalByVin() const = 0;
  size_t countTotalByYear() const;
  size_t countTotalByModel() const;
  virtual const string_view& getName() const = 0;
  uint64_t getInitTime() const { return m_initTime; }
  virtual void init( const std::vector<Vehicle>& vehicleSet ) = 0;
  virtual void print( ostream& ios ) const = 0;
  virtual size_t searchByYear( uint16_t year ) const = 0;
  virtual size_t searchByModel( Vehicle::e_Model model ) const = 0;
  virtual bool searchByVin( const std::string& vin) const = 0;
  virtual void traverse() const = 0;
  virtual void traverseAndWrite() = 0;

  std::string m_fileName;
  bool m_verbose;
  uint64_t m_initTime = 0;

private:
  VehicleRepo();
  VehicleRepo( const VehicleRepo& v );
};

#endif /* VEHICLEREPO_HPP_ */
