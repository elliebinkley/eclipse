/*
 * vehicle.hpp
 *
 *  Created on: Oct 21, 2019
 *      Author: laurence
 */

#ifndef VEHICLEREPOSHRPTR_HPP_
#define VEHICLEREPOSHRPTR_HPP_

#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "Vehicle.hpp"
#include "VehicleRepo.hpp"

/*
 * A vehicle repository that implements interface VehicleRepo using
 * std::multimap and std::map and shared pointers to common data
 */

class VehicleRepoShrPtr : public VehicleRepo
{
public:

  typedef std::map<std::string, std::shared_ptr<Vehicle>> StdMap;
  typedef std::multimap<std::string, std::shared_ptr<Vehicle>> StdMultiMap;

  static constexpr std::string_view m_Name = "VehicleRepoShrPtr";

  //ctor anbd dtor
  VehicleRepoShrPtr( bool verbose ) : VehicleRepo( verbose ) {}
  virtual ~VehicleRepoShrPtr() = default;

  void cleanup();
  inline size_t countTotalByVin() const { return m_repoShrPtrByVin.size (); }
  inline const string_view& getName() const { return m_Name; }
  void init( const std::vector<Vehicle>& vehicleSet );
  void print( ostream& ios ) const;
  size_t searchByYear( uint16_t year ) const;
  size_t searchByModel( Vehicle::e_Model model ) const;
  bool searchByVin( const std::string& vin ) const;
  void traverse() const;
  void traverseAndWrite();

private:
  VehicleRepoShrPtr();                                               // no implementation
  VehicleRepoShrPtr( const VehicleRepoShrPtr& repo );                // no implementation
  VehicleRepoShrPtr& operator = (const VehicleRepoShrPtr& other);    // no implementation

  bool add( Vehicle vehicle );
  bool add( const std::string& vin, const Vehicle::e_Manufacturer manufacturer,
       const Vehicle::e_Model model, const Vehicle::e_Color color,
       uint16_t year );
  bool remove( std::string& vin );

  // masterRepo; unique by VIN; use map (instead of set) so that the values are modifiable.
  StdMap m_repoShrPtrByVin;

  // keyed by year
  std::multimap<uint16_t, std::shared_ptr<Vehicle>> m_repoShrPtrByYear;

  // keyed by model
  std::multimap<Vehicle::e_Model, std::shared_ptr<Vehicle>> m_repoShrPtrByModel;
};

#endif /* VEHICLEREPOSHRPTR_HPP_ */
