/*
 * VehicleRepoIntrusive.hpp
 *
 *  Created on: Nov 3, 2019
 *      Author: laurence
 */

#ifndef VEHICLEREPO_INTRUSIVE_HPP_
#define VEHICLEREPO_INTRUSIVE_HPP_

#include <boost/intrusive/set.hpp>
#include <boost/intrusive/unordered_set.hpp>
#include "VehicleIntrusive.hpp"
#include "VehicleRepo.hpp"

using namespace boost::intrusive;



/*
 * A vehicle repository that implements interface VehicleRepo using
 * boost::intrusive::set and boost::intrusive::multiset
 * Uses boost:intrusive map-like interface.
 * The map-like interface requires:
 *  1. key_of_value function object, must:
 *     - be default constructible
 *     - define the key type using "type"
 *     - define an operator() taking "const value_type&" and
 *       returning "type" or "const type &"
 */

class VehicleRepoIntrusive : public VehicleRepo
{
public:
   // function objects for different key values, e.g. year, model vin
   struct year_is_key
   {
     typedef uint16_t type;
     type operator() (const VehicleIntrusive& v) const { return v.getYear(); }
   };

   struct model_is_key
   {
     typedef Vehicle::e_Model type;
     type operator() (const VehicleIntrusive& v) const { return v.getModel(); }
   };

   struct vin_is_key
   {
     typedef std::string type;
     type operator() (const VehicleIntrusive& v) const { return v.getVin (); }
   };

   // key by VIN
   typedef boost::intrusive::member_hook<VehicleIntrusive,
       boost::intrusive::set_member_hook<>, &VehicleIntrusive::m_vinMember_hook> VinMemberOption;

   typedef boost::intrusive::set<VehicleIntrusive, VinMemberOption,
       key_of_value<vin_is_key>> VinMemberSet;

   // key by year
   typedef boost::intrusive::member_hook<VehicleIntrusive,
       boost::intrusive::set_member_hook<>, &VehicleIntrusive::m_yearMember_hook> YearMemberOption;

   typedef boost::intrusive::multiset<VehicleIntrusive, YearMemberOption,
       key_of_value<year_is_key> > YearOrderedMap;

   // key by Model
   typedef boost::intrusive::member_hook<VehicleIntrusive,
       boost::intrusive::set_member_hook<>, &VehicleIntrusive::m_modelMember_hook> ModelMemberOption;

   typedef boost::intrusive::multiset<VehicleIntrusive, ModelMemberOption,
       key_of_value<model_is_key> > ModelOrderedMap;

   //The disposer object function for cleaning up memory in the intrusive repositories.
   struct disposer
   {
   public:
     void operator () (VehicleIntrusive *dispose_this)
     {
       dispose_this->~VehicleIntrusive();
     }
   };

   static constexpr std::string_view m_Name = "VehicleRepoIntrusive";

   VehicleRepoIntrusive( bool verbose ) :
      VehicleRepo( verbose ),
      m_vPtr( 0 ),
      m_repoIntrusiveByVin(),
      m_repoIntrusiveByYear(),
      m_repoIntrusiveByModel() {}
  virtual ~VehicleRepoIntrusive();
  void cleanup();
  inline size_t countTotalByVin() const { return m_repoIntrusiveByVin.size(); }
  inline const string_view& getName() const { return m_Name; }
  void init( const std::vector<Vehicle>& vehicleSet );
  void print( ostream& ios ) const;
  size_t searchByModel( Vehicle::e_Model model ) const;
  size_t searchByYear( uint16_t year ) const;
  bool searchByVin( const std::string& vin ) const;
  void traverse() const;
  void traverseAndWrite();

private:
  VehicleRepoIntrusive();                                                  // no implementation
  VehicleRepoIntrusive( const VehicleRepoIntrusive& repo );                // no implementation
  VehicleRepoIntrusive& operator = (const VehicleRepoIntrusive& other);    // no implementation

  // Define the start of the contiguous memory buffer for all vehicles in filename
  VehicleIntrusive *m_vPtr;

  //Define intrusive sets
  VinMemberSet m_repoIntrusiveByVin;
  YearOrderedMap m_repoIntrusiveByYear;
  ModelOrderedMap m_repoIntrusiveByModel;

};

#endif /* VEHICLEREPO_INTRUSIVE_HPP_ */
