/*
 * Vehicle_Intrusive.hpp
 *
 *  Created on: Nov 3, 2019
 *      Author: laurence
 */

#ifndef VEHICLE_INTRUSIVE_HPP_
#define VEHICLE_INTRUSIVE_HPP_

#include "Vehicle.hpp"
#include <boost/intrusive/set_hook.hpp>
#include <boost/intrusive/unordered_set_hook.hpp>
#include <functional>
#include <iostream>

using namespace boost::intrusive;

// Intrusive version of Vehicle; used for intrusive containers.
class VehicleIntrusive : public Vehicle
{
using Vehicle::Vehicle;
public:

  // Copy constructor;
  VehicleIntrusive( const Vehicle& v ) : Vehicle( v ) {}

  // Copy constructor; don't copy over set_member_hook
  VehicleIntrusive (const VehicleIntrusive& v) : Vehicle (v) {}

  // Move CTOR; don't copy over set_member_hook
  VehicleIntrusive (VehicleIntrusive&& v) noexcept : Vehicle (v) {}

  // move assignment operator; don't copy over set_member_hook
  VehicleIntrusive& operator = (const VehicleIntrusive& other)
  {
    if (this == &other) return (*this);
    ((Vehicle&) *this) = other;
    return (*this);
  }

  // relational operators; use base class operators
  inline bool operator == (const VehicleIntrusive& other)
    { return( ((VehicleIntrusive&) *this) == other ); }
  inline bool operator <  (const VehicleIntrusive& other)
    { return( ((VehicleIntrusive&) *this) < other );  }
  inline bool operator <= (const VehicleIntrusive& other)
    { return( ((VehicleIntrusive&) *this) <= other ); }
  inline bool operator >  (const VehicleIntrusive& other)
    { return( ((VehicleIntrusive&) *this) > other );  }
  inline bool operator >= (const VehicleIntrusive& other)
    { return( ((VehicleIntrusive&) *this) >= other ); }
  inline bool operator != (const VehicleIntrusive& other)
    { return( ((VehicleIntrusive&) *this) != other ); }

  inline friend bool operator == (const VehicleIntrusive& lhs, const VehicleIntrusive& rhs)
    { return( ((Vehicle&) lhs) == ((Vehicle&) rhs) ); }
  inline friend bool operator <  (const VehicleIntrusive& lhs, const VehicleIntrusive& rhs)
    { return( ((Vehicle&) lhs) < ((Vehicle&) rhs) ); }
  inline friend bool operator <= (const VehicleIntrusive& lhs, const VehicleIntrusive& rhs)
    { return( ((Vehicle&) lhs) <= ((Vehicle&) rhs) ); }
  inline friend bool operator >= (const VehicleIntrusive& lhs, const VehicleIntrusive& rhs)
    { return( ((Vehicle&) lhs) >= ((Vehicle&) rhs) ); }
  inline friend bool operator >  (const VehicleIntrusive& lhs, const VehicleIntrusive& rhs)
    { return (((Vehicle&) lhs) > ((Vehicle&) rhs)); }
  inline friend bool operator!= (const VehicleIntrusive& lhs, const VehicleIntrusive& rhs)
    { return ( ((Vehicle&) lhs) != ((Vehicle&) rhs) ); }

  friend std::size_t hash_value( const VehicleIntrusive& value )
  {
    return( std::hash<std::string>{} ( value.getVin() ) );
  }

  virtual ~VehicleIntrusive ()
  {
    if( Vehicle::m_verbose ) std::cout << "VehicleIntrusive DTOR" << std::endl;
  }

  boost::intrusive::set_member_hook<> m_vinMember_hook;
  boost::intrusive::set_member_hook<> m_yearMember_hook;
  boost::intrusive::set_member_hook<> m_modelMember_hook;

private:
  VehicleIntrusive ();                  // no implementation

};

#endif /* VEHICLE_INTRUSIVE_HPP_ */
