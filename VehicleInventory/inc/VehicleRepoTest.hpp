/*
 * VehicleRepoTest.hpp
 *
 *  Created on: Nov 21, 2019
 *      Author: laurence
 */

#ifndef VEHICLEREPOTEST_HPP_
#define VEHICLEREPOTEST_HPP_

#include <chrono>
#include <map>
#include <memory>
#include <vector>

#include "../inc/Vehicle.hpp"
#include "../inc/VehicleRepo.hpp"

/*
 * This is class is used to orchestrate the testing of various repositories
 * It creates the repositories, initializes them with "vehicle" objects and
 * then runs performance tests ( see s_TestSequence ) on the repositories.
 */

class VehicleRepoTest
{
public:

  enum e_TestId
  {
       INIT,
       TRAVERSE,
       TRAVERSE_WRITE,
       SEARCH_VIN,
       SEARCH_YEAR,
       SEARCH_MODEL,
       LAST_TEST_ID
  };

  const typedef struct
  {
    e_TestId testID;
    const char *testName;
  } s_TestSequence;

  static constexpr std::array<VehicleRepoTest::s_TestSequence, LAST_TEST_ID> m_testSequences =
  {{
  { e_TestId::INIT, "initialization" },
  { e_TestId::TRAVERSE, "traverse" },
  { e_TestId::TRAVERSE_WRITE, "traverseWrite" },
  { e_TestId::SEARCH_VIN, "searchByVin" },
  { e_TestId::SEARCH_YEAR, "searchByYear" },
  { e_TestId::SEARCH_MODEL, "searchByModel" }
  }};

  static constexpr uint32_t MAX_VEHICLES = 10000000;

  VehicleRepoTest () = default;
  ~VehicleRepoTest () = default;
  void audit ();
  void cleanUp ();
  void createRepos ();
  void createVehicles ();
  size_t enterNumVehicles ();
  void initRepo ();
  void printIntro () const;
  void printRandomVehicleVin () const;
  void printResults () const;
  void run ();
  void searchByYear ();
  void searchByModel ();
  void searchByVin ();
  void traverse ();
  void traverseAndWrite ();

  class TestResult   //inner container class to hold results of tests
  {
  public:
    TestResult (e_TestId id, std::string repo, uint64_t result) :
	m_testID (id), m_repoName (repo), m_result (result)  {}

    // members
    e_TestId m_testID;
    std::string m_repoName;
    uint64_t m_result;
  };

private:
  // file to create of generated vehicles for user viewing
  std::string m_VehicleFileName = "vehicleRepo.txt";

  // list of vehicles to feed each repo
  std::vector<Vehicle> m_vehicles;

  // list of repos to test
  std::vector<std::unique_ptr<VehicleRepo>> m_repoList;

  // list of test results
  std::vector<TestResult> m_results;

  bool m_verbose = false;

  // number of vehicles created that are not dups.
  uint32_t m_numVehicles = 0;
};

#endif /* VEHICLEREPOTEST_HPP_ */
