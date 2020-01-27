/*
 * VehicleRepoShrPtrUnordered.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: laurence
 */

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

#include "../inc/VehicleFile.hpp"
#include "../inc/VehicleRepoShrPtrUnordered.hpp"



void
VehicleRepoShrPtrUnordered::init (const std::vector<Vehicle> &vehicleSet)
{

  // insert vehicles into data structures
  std::chrono::system_clock::time_point before =
      std::chrono::system_clock::now ();
  size_t localCount = 0;
  uint32_t numDups = 0;
  for (const auto &vehicle : vehicleSet)
    {
      try
	{
	  // make a pair (vin,shared_ptr to vehicle)
	  std::pair<std::string, std::shared_ptr<Vehicle>> pair (
	      vehicle.getVin (),
	      std::shared_ptr<Vehicle> (new Vehicle (vehicle)));

	  // insert pair into map keyed by VIN
	  auto ret = m_repoShrPtrUnorderedByVin.insert (pair);
	  if (!ret.second)
	    {
	      numDups++;
	      continue;
	    }

	  // insert into multimap keyed by year
	  uint16_t year = pair.second->getYear ();
	  std::pair<uint16_t, std::shared_ptr<Vehicle> > pairYear =
	      std::make_pair (year, std::move (pair.second));
	  m_repoShrPtrUnorderedByYear.insert (pairYear);

	  // insert into multimap keyed by model
	  Vehicle::e_Model model = pairYear.second->getModel ();
	  std::pair<Vehicle::e_Model, std::shared_ptr<Vehicle> > pairModel =
	      std::make_pair (model, std::move (pairYear.second));
	  m_repoShrPtrUnorderedByModel.insert (pairModel);
	}
      catch (std::string &e)
	{
	  std::cerr << "catch():" << e << std::endl;
	}
      catch (std::exception &e)
	{
	  std::cerr << "catch():" << e.what () << std::endl;
	}
      catch (...)
	{
	  std::cerr << "catch():" << std::endl;
	}
      localCount++;
    }
  std::chrono::system_clock::time_point after =
      std::chrono::system_clock::now ();

  m_initTime = (std::chrono::duration_cast<std::chrono::microseconds>) (
      after - before).count ();

  cout << "\n VehicleRepoShrPtrUnordered:" << __FUNCTION__ << "\n insertions:"
      << localCount << " duplicates:" << numDups << "\n took " << m_initTime
      << "us." << endl;

  cout << "m_repoShrPtrUnorderedByModel.size():"
      << m_repoShrPtrUnorderedByModel.size () << endl;
  cout << "m_repoShrPtrUnorderedByYear.size():"
      << m_repoShrPtrUnorderedByYear.size () << endl;
  cout << "m_repoShrPtrUnorderedByVin.size():"
      << m_repoShrPtrUnorderedByVin.size () << endl;

  audit ();  // do not include audit in the time.

  /* For m_repoShrPtrUnorderedByModel only 11 buckets ( one for each of the 11 models)
   * will store all the elements, but apparently
   * a bucket is created for every element, meaning that for 100000 elements,all but 11 are empty...
   * Likewise for m_repoShrPtrUnorderedByYear......
   *  cout << "m_repoShrPtrUnorderedByModel.bucket_count():" << m_repoShrPtrUnorderedByModel.bucket_count() << endl;
   * 	cout << "m_repoShrPtrUnorderedByYear.bucket_count():" << m_repoShrPtrUnorderedByYear.bucket_count() << endl;
   * 	cout << "m_repoShrPtrUnorderedByVin.bucket_count():" << m_repoShrPtrUnorderedByVin.bucket_count() << endl;
   *   for (int i = 0; i < m_repoShrPtrUnorderedByModel.bucket_count(); ++i)
   * 	      cout << "Bucket " << i << " contains "
   *             << m_repoShrPtrUnorderedByModel.bucket_size(i) << " elements." << endl;
   */
}

bool
VehicleRepoShrPtrUnordered::remove (std::string &vin)
{
  // find the element
  std::unordered_map<std::string, std::shared_ptr<Vehicle>>::iterator it;
  it = m_repoShrPtrUnorderedByVin.find (vin);
  if (it == m_repoShrPtrUnorderedByVin.end ())
    {
      return false;  // not found
    }

  // delete from year map
  uint16_t year = it->second->getYear ();

    {
      auto range = m_repoShrPtrUnorderedByYear.equal_range (year);
      for (auto it1 = range.first; it1 != range.second; it1++)
	{
	  if (it1->second.get ()->getVin () == vin)
	    m_repoShrPtrUnorderedByYear.erase (it1);
	}
    }

  // delete from model map
  Vehicle::e_Model model = it->second->getModel ();
    {
      auto range = m_repoShrPtrUnorderedByModel.equal_range (model);
      for (auto it1 = range.first; it1 != range.second; it1++)
	{
	  if (it1->second.get ()->getVin () == vin)
	    m_repoShrPtrUnorderedByModel.erase (it1);
	}
    }

  // delete from VIN map
  m_repoShrPtrUnorderedByVin.erase (it);
  return true;
}

bool
VehicleRepoShrPtrUnordered::add (const std::string &vin,
				 const Vehicle::e_Manufacturer manufacturer,
				 const Vehicle::e_Model model,
				 const Vehicle::e_Color color, uint16_t year)
{
  return add (Vehicle (vin, model, color, year));
}

bool
VehicleRepoShrPtrUnordered::add (Vehicle vehicle) // make a copy? TODO see if reference will work.
{
  bool retVal = false;
  // Forces map key to have the same key (VIN) as in vehicle class. ( no dups allowed)
  std::shared_ptr<Vehicle> ptr = make_shared<Vehicle> (vehicle);
  auto pair = std::make_pair (ptr->getVin (), std::move (ptr));
  std::pair<std::unordered_map<std::string, std::shared_ptr<Vehicle>>::iterator,
      bool> ret;
  ret = m_repoShrPtrUnorderedByVin.insert (pair);
  if (ret.second)
    {
      cout << " insert success for VIN=" << ret.first->second.get ()->getVin ()
	  << endl;
    }
  else
    {
      cout << "insert failed ( possible duplicate..) for VIN="
	  << vehicle.getVin () << endl;
      return false;
    }

  // insert to the year map
  std::shared_ptr<Vehicle> ptrYear = make_shared<Vehicle> (vehicle);
  auto pairYear = std::make_pair (ptrYear->getYear (), std::move (ptrYear));
  m_repoShrPtrUnorderedByYear.insert (pairYear);

  // insert to the model map
  std::shared_ptr<Vehicle> ptrModel = make_shared<Vehicle> (vehicle);
  auto pairModel = std::make_pair (ptrModel->getModel (), std::move (ptrModel));
  m_repoShrPtrUnorderedByModel.insert (pairModel);

  return retVal;
}

void
VehicleRepoShrPtrUnordered::print (ostream &ios) const
{

  if (m_verbose)
    {
      ios
	  << "..........printing entire repository  for VehicleRepoShrPtrUnordered ..............."
	  << endl;
      ios << "..........printing by VIN ..............." << endl;
      for (const auto &car : m_repoShrPtrUnorderedByVin)
	{
	  car.second.get ()->printAsString (ios);
	}
      ios << "..........printing by YEAR..............." << endl;
      for (const auto &car : m_repoShrPtrUnorderedByYear)
	{
	  car.second.get ()->printAsString (ios);
	}
      ios << "..........printing by Model..............." << endl;
      for (const auto &car : m_repoShrPtrUnorderedByModel)
	{
	  car.second.get ()->printAsString (ios);
	}
      assert(
	  m_repoShrPtrUnorderedByVin.size ()
	      == m_repoShrPtrUnorderedByYear.size ());
      assert(
	  m_repoShrPtrUnorderedByVin.size ()
	      == m_repoShrPtrUnorderedByModel.size ());
    }
  ios << "m_repoShrPtrUnorderedByVin/Model/Year has "
      << m_repoShrPtrUnorderedByVin.size () << " items" << endl;
}

size_t
VehicleRepoShrPtrUnordered::searchByModel (Vehicle::e_Model model) const
{
  auto range = m_repoShrPtrUnorderedByModel.equal_range (model);
  if (range.first == m_repoShrPtrUnorderedByModel.end ())
    return 0;

  if (m_verbose)
    {
      for (auto it = range.first; it != range.second; it++)
	{
	  assert(it->second->getModel () == model);
	  it->second->printAsString (cout);
	}
    }
  size_t numFound = std::distance (range.first, range.second);
  return numFound;
}

size_t
VehicleRepoShrPtrUnordered::searchByYear (uint16_t year) const
{
  auto range = m_repoShrPtrUnorderedByYear.equal_range (year);
  if (range.first == m_repoShrPtrUnorderedByYear.end ())
    return 0;

  if (m_verbose)
    for (auto it = range.first; it != range.second; it++)
      {
	assert(it->second->getYear () == year);
	it->second->printAsString (cout);
      }

  size_t numFound = std::distance (range.first, range.second);
  return numFound;
}

bool
VehicleRepoShrPtrUnordered::searchByVin (const std::string &vin) const
{
  auto element = m_repoShrPtrUnorderedByVin.find (vin);
  if (element == m_repoShrPtrUnorderedByVin.end ())
    return 0;

  if (element == m_repoShrPtrUnorderedByVin.end ())
    return false;
  if (element->second->getVin () != vin)
    return false;

  element->second->printAsString (cout);
  return true;
}

void
VehicleRepoShrPtrUnordered::cleanup ()
{
  // delete shared pointers/elements ; last share pointer should delete object.

  cout << "cleaning up m_repoShrPtrUnorderedByVin" << endl;
  m_repoShrPtrUnorderedByVin.erase (m_repoShrPtrUnorderedByVin.begin (),
				    m_repoShrPtrUnorderedByVin.end ());
  assert(m_repoShrPtrUnorderedByVin.empty ());

  // TODO validate that a bucket erase frees the elements in the bucket.
  cout << "cleaning up m_repoShrPtrUnorderedByYear" << endl;
  m_repoShrPtrUnorderedByYear.erase (m_repoShrPtrUnorderedByYear.begin (),
				     m_repoShrPtrUnorderedByYear.end ());
  assert(m_repoShrPtrUnorderedByYear.empty ());

  cout << "cleaning up m_repoShrPtrUnorderedByModel" << endl;
  m_repoShrPtrUnorderedByModel.erase (m_repoShrPtrUnorderedByModel.begin (),
				      m_repoShrPtrUnorderedByModel.end ());
  assert(m_repoShrPtrUnorderedByModel.empty ());
}

void
VehicleRepoShrPtrUnordered::traverse () const
{
  for (const auto &element : m_repoShrPtrUnorderedByVin)
    {
      element.second->getScratch ();
    }
}

void
VehicleRepoShrPtrUnordered::traverseAndWrite ()
{
  for (auto &element : m_repoShrPtrUnorderedByVin)
    {
      element.second->incrementScratch ();
    }
}

