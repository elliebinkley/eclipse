/*
 * TstVehicleRepo.hpp
 *
 *  Created on: Oct 24, 2019
 *      Author: laurence
 *
 */

#ifndef VEHICLEREPOVECTOR_HPP_
#define VEHICLEREPOVECTOR_HPP_

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "Vehicle.hpp"
#include "VehicleRepo.hpp"

/*
 * A vehicle repository that implements interface VehicleRepo using std::vector
 */

class VehicleRepoVector : public VehicleRepo
{
public:
  static constexpr std::string_view m_Name = "VehicleRepoVector";

  VehicleRepoVector( bool verbose ) : VehicleRepo (verbose) {}
  virtual ~VehicleRepoVector() = default;

  void cleanup ();
  inline size_t countTotalByVin() const { return m_repoVector.size(); }
  inline const string_view& getName() const { return m_Name; }
  void init (const std::vector<Vehicle>& vehicleSet);
  void printRandomVehicleVin() const;
  void print( ostream& ios ) const;
  size_t searchByYear( uint16_t year) const;
  size_t searchByModel( Vehicle::e_Model model ) const;
  bool searchByVin( const std::string &vin ) const;
  void traverse() const;
  void traverseAndWrite();

private:
  VehicleRepoVector();                                               // no implementation
  VehicleRepoVector(const VehicleRepoVector& v);                     // no implementation
  VehicleRepoVector& operator = (const VehicleRepoVector& other);    // no implementation

  std::vector<Vehicle> m_repoVector;
};
#endif /* VEHICLEREPOVECTOR_HPP_ */
