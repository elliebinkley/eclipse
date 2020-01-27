/*
 * VehicleRepoShrPtrUnordered.hpp
 *
 *  Created on: Nov 18, 2019
 *      Author: laurence
 */

#ifndef VEHICLEREPOSHRPTRUNORDERED_HPP_
#define VEHICLEREPOSHRPTRUNORDERED_HPP_

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <string>
#include <string_view>
#include "Vehicle.hpp"
#include "VehicleRepo.hpp"

/*
 * A vehicle repository that implements interface VehicleRepo using
 * std::unordered map and std::unordered_multimap
 */

class VehicleRepoShrPtrUnordered : public VehicleRepo
{
public:
  static constexpr std::string_view m_Name = "VehicleRepoShrPtrUnordered";

  VehicleRepoShrPtrUnordered( bool verbose ) : VehicleRepo( verbose ) {}
  virtual ~VehicleRepoShrPtrUnordered() = default;

  void cleanup();
  inline size_t countTotalByVin() const { return m_repoShrPtrUnorderedByVin.size(); }
  inline const string_view& getName() const { return m_Name; }
  void init (const std::vector<Vehicle>& vehicleSet);
  void print (ostream& ios) const;
  size_t searchByYear( uint16_t year ) const;
  size_t searchByModel( Vehicle::e_Model model ) const;
  bool searchByVin( const std::string& vin ) const;
  void traverse() const;
  void traverseAndWrite();

private:
  VehicleRepoShrPtrUnordered();  // no implementation
  VehicleRepoShrPtrUnordered( const VehicleRepoShrPtrUnordered& repo );                // no implementation
  VehicleRepoShrPtrUnordered& operator = (const VehicleRepoShrPtrUnordered& other);    // no implementation

  bool add( Vehicle vehicle );
  bool add( const std::string &vin, const Vehicle::e_Manufacturer manufacturer,
       const Vehicle::e_Model model, const Vehicle::e_Color color,
       uint16_t year );
  bool remove( std::string &vin );

  // masterRepo; unique by VIN; use map instead of set so that the values are modifiable.
  std::unordered_map<std::string, std::shared_ptr<Vehicle>> m_repoShrPtrUnorderedByVin;

  // keyed by year
  std::unordered_multimap<uint16_t, std::shared_ptr<Vehicle>> m_repoShrPtrUnorderedByYear;

  // keyed by model
  std::unordered_multimap<Vehicle::e_Model, std::shared_ptr<Vehicle>> m_repoShrPtrUnorderedByModel;
};

#endif /* VEHICLEREPOSHRPTRUNORDERED_HPP_ */
