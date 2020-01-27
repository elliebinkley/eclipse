/*
 * VehicleRepoTest.cpp
 *
 *  Created on: Nov 21, 2019
 *      Author: laurence
 */

#include <cstdlib>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "../inc/VehicleFile.hpp"
#include "../inc/VehicleRepoIntrusive.hpp"
#include "../inc/VehicleRepoShrPtr.hpp"
#include "../inc/VehicleRepoShrPtrUnordered.hpp"
#include "../inc/VehicleRepoTest.hpp"
#include "../inc/VehicleRepoVector.hpp"

void VehicleRepoTest::run()
{
   cout << "run start" << endl;
   try
   {
      printIntro();
      createRepos();
      createVehicles();
      initRepo();
      audit();
      traverse();
      traverseAndWrite();
      searchByVin();
      searchByYear();
      searchByModel();
      printResults();

      cleanUp();
   }
   catch( std::string& e )
   {
      std::cerr << "catch():" << e << std::endl;
   }
   catch( std::exception& e )
   {
      std::cerr << "catch():" << e.what() << std::endl;
   }
   catch( ... )
   {
      std::cerr << "catch():" << std::endl;
   }
   cout << "run end" << endl;
}

// run a quick audit for consistency
void VehicleRepoTest::audit()
{
   // make sure all Year repositories have same number of vehicles
   {
      size_t oldCount = 0;
      size_t newCount = 0;
      for( const auto &repo : m_repoList )
      {
         newCount = repo->countTotalByYear();
         if( (oldCount != 0) && (oldCount != newCount) )
         {
            std::cout << "mismatch on totalByYear=" << " count=" << oldCount
                  << " newCount=" << newCount << std::endl;
         }
         oldCount = newCount;
      }
   }
   // make sure all Model repositories have same number of vehicles
   {
      size_t oldCount = 0;
      size_t newCount = 0;
      for( const auto &repo : m_repoList )
      {
         newCount = repo->countTotalByModel();
         if( (oldCount != 0) && (oldCount != newCount) )
         {
            std::cout << "mismatch on totalByModel=" << " count=" << oldCount
                  << " newCount=" << newCount << std::endl;
         }
         oldCount = newCount;
      }
   }
   // make sure all Vin repositories have same number of vehicles
   {
      size_t oldCount = 0;
      size_t newCount = 0;
      for( const auto &repo : m_repoList )
      {
         newCount = repo->countTotalByVin();
         if( (oldCount != 0) && (oldCount != newCount) )
         {
            std::cout << "mismatch on totalByVind=" << " count=" << oldCount
                  << " newCount=" << newCount << std::endl;
         }
         oldCount = newCount;
      }
      m_numVehicles = newCount;
   }
}

void VehicleRepoTest::cleanUp()
{
   for( auto &iter : m_repoList )
   {
      Vehicle::clearCounts();
      iter->cleanup();
      Vehicle::printCounts( std::cout );
   }
   m_repoList.clear();
}
;

void VehicleRepoTest::createRepos()
{
   m_repoList.push_back(
         std::unique_ptr<VehicleRepoVector>(
               new VehicleRepoVector( m_verbose ) ) );
   m_repoList.push_back(
         std::unique_ptr<VehicleRepoShrPtr>(
               new VehicleRepoShrPtr( m_verbose ) ) );
   m_repoList.push_back(
         std::unique_ptr<VehicleRepoShrPtrUnordered>(
               new VehicleRepoShrPtrUnordered( m_verbose ) ) );
   m_repoList.push_back(
         std::unique_ptr<VehicleRepoIntrusive>(
               new VehicleRepoIntrusive( m_verbose ) ) );
}

void VehicleRepoTest::createVehicles()
{
// create a file of and a vector of randomly generated vehicle objects
   size_t number = enterNumVehicles();
   VehicleFile vehicleFile( m_VehicleFileName, m_verbose );
   vehicleFile.createVehicleFile( number, std::cout );
   this->m_vehicles = std::move( vehicleFile.getVehicleVector() );
   Vehicle::printCounts( std::cout );
   Vehicle::clearCounts();
   cout << "vehicleFile.size()" << vehicleFile.getVehicleVector().size()
         << endl;
   cout << "this->m_vehicles.size()" << this->m_vehicles.size() << endl;
}

size_t VehicleRepoTest::enterNumVehicles()
{
   size_t number = 0;
   std::cout << "How many vehicles do you want in the repositories:(Enter 10-"
         << VehicleRepoTest::MAX_VEHICLES << ")?" << std::endl;
   while( true )
   {
      std::string input;
      std::cout << "enter number of Vehicles:(e.g. 100,000)" << std::endl;
      std::cin >> input;
      // check for all digits in input

      string::const_iterator itr = input.begin();
      while( itr != input.end() && isdigit( *itr ) )
         ++itr;
      if( !(!input.empty() && itr == input.end()) )
      {
         // not a digit
         cout << input << " is not a number " << endl;
         std::cerr << "Enter 10-" << VehicleRepoTest::MAX_VEHICLES << endl;
         continue;
      }
      number = std::atoi( input.c_str() );
      if( number < 10 || number > VehicleRepoTest::MAX_VEHICLES )
      {
         std::cerr << "out-of-range:" << number << endl;
         std::cerr << "Valid range:10-" << VehicleRepoTest::MAX_VEHICLES
               << endl;
         continue;
      }
      else
      {
         break;
      }
   }
   std::cout << "Number of Vehicles chosen=" << number << std::endl;
   return number;
}
;

void VehicleRepoTest::initRepo()
{
   cout << "\n\n\n" << endl;
   cout << " Task 1: Initialize all Repositories...." << endl;
   for( auto &repo : m_repoList )
   {
      Vehicle::clearCounts();

      repo->init( m_vehicles );

      repo->print( std::cout );
      Vehicle::printCounts( std::cout );
      Vehicle::clearCounts();

      TestResult result( VehicleRepoTest::INIT, std::string( repo->getName() ),
                         repo->getInitTime() );
      m_results.push_back( result );

      cout << "\n" << endl;
   }
}

void VehicleRepoTest::printIntro() const
{
   std::cout
         << "This program evaluates performance of various containers for \n"
         << "creating, searching, traversing and writing Vehicle objects \n"
         << "Various Vehicle repositories are created \n"
         << "1. VehicleRepoVector; uses a single vector data structure sorted by VIN \n"
         << "   for searching for vehicles by VIN, Year or Model. \n"
         << "2. VehicleRepoShrPtrUnordered; has 3 data structures all using shared pointers to a \n"
         << "   single set of Vehicle objects. \n"
         << "   a. std:unordered_map to sort Vehicles by unique VIN. \n"
         << "   b. std:unordered_multimap to sort Vehicles by non-unique car model. \n"
         << "   c. std:unordered_multimap to sort Vehicles by non-unique year. \n"
         << "3. VehicleRepoShrPtr; has 3 data structures all using shared pointers to a  \n"
         << "   single set of Vehicle objects. \n"
         << "   a. std:map to sort Vehicles by unique VIN. \n"
         << "   b. std:multimap to sort Vehicles by non-unique car model. \n"
         << "   c. std:multimap to sort Vehicles by non-unique year. \n"
         << "4. VehicleIntrusiveRepo; has 3 data structures all sharing the same set of \n"
         << "   VehicleIntrusive objects where VehicleIntrusive inherits from Vehicle. \n"
         << "   VehicleIntrusive objects are allocated in contiguous memory to improve locality( cache hits).\n"
         << "   a. boost::intrusive::set to store vehicle by unique VIN. \n"
         << "   b. boost::intrusive::multiset to store vehicles by non-unique car model. \n"
         << "   c. boost::intrusive::multiset to store vehicles by non-unique year. \n"
         << "   These 3 data structures use the map like intrusive interface based on \n"
         << "   on key_of_value<> . \n" << " Tasks:"
         << " 1. The first task is to create repositories 1-4 from a generated file that randomly generates\n"
         << "    vehicle objects. There may be some vehicles that have duplicate VINs. \n"
         << "    The dups are eliminated and all repositories except unordered sets are sorted. Note that set of \n"
         << "    vehicles in Repo#1 may differ slightly from Repos in #2-#4 because the way duplicates to be erased \n"
         << "    are chosen in Repo#1 differs from that in Repo#2-#4. However, the total number of Vehicles \n"
         << "    is the same in all repositories. "
         << "    Insertion/creation is timed and measured with Repo#1 being the winner. \n"
         << "    But since Repo#1 copies objects, its performance suffers. \n"
         << "    Interestingly, Repo#4 takes the longest to create, even though the VehicleIntrusives memory is allocated \n"
         << "    all at once instead or per object creation like #2-#3. Presumably, VehicleIntrusive creation \n"
         << "    takes longer because of the three intrusive hooks overhead in each VehicleIntrusive instantiation.\n"
         << " \n"
         << " 2. The second task is to iterate across all elements in each repo using 1, 2a, 3a and 4a. \n"
         << "    This is timed and measured with #1 being the winner. \n"
         << "    Presumably, pointer arithmetic is used on Repo#1 since it is a vector thus it is fast, while\n"
         << "    2a-4a are tree traversals. \n"
         << "   IntrusiveRepo (#4a) beats 2a and 3a, due to cache locality.\n"
         << " \n"
         << " 3. The third task is to iterate across all elements in each Repo using 1, 2a, 3a and 4a. \n"
         << "    and writing the m_scratch field in each vehicle object. \n"
         << "    This is timed and measured with #1 being the winner with results similar to the second task. \n"
         << " \n"
         << " 4. The fourth task is to search Repo #1-#4 by VIN using 1, 2a, 3a and 4a. \n"
         << "    This is timed and measured with #2a being the winner, as expected, since \n"
         << "    2a is a direct hash lookup. \n" << " \n"
         << " 5. The fifth task is to search Repo #1-#4 by Year using 1, 2c, 3c and 4c. These return multiple items \n"
         << "    in an iterator using multiset, multimap structures. \n"
         << "    This is timed and measured with #3c being the winner \n"
         << "    Repo #1 performs horribly due to linear searching and copying of Vehcile objects.\n"
         << "    Repo #2 performs badly because the hash function finds a bucket of Vehicles corresponding to that year\n"
         << "    and then seems to iterate linearly over the items in that bucket.\n"
         << "6.  The sixth task is to search Repo #1-#4 by vehicle Model using 1, 2b, 3b and 4b. \n"
         << "    This is very similar to task 5 and has similar results. \n"
         << "7.  Summary:"
         << "    Repo#3 and Repo#4 perform the best overall primarily because this application requires \n"
         << "    handling of ranges and tasks 5 and 6 perform far better than Repo #1 and #2.\n"
         << "    This makes up for slight advantages that Repo#1 and #2 have in other scenarios.\n"
         << std::endl;
   return;
}

void VehicleRepoTest::printResults() const
{
   std::cout << "\n\nPrinting Results for " << m_numVehicles << " vehicles "
         << std::endl;
   for( u_int16_t idx = 0; idx < VehicleRepoTest::LAST_TEST_ID; idx++ )
   {
      std::vector<TestResult> s_searchByIdx;
      // copy matching results to s_searchByTestId;
      std::copy_if( this->m_results.begin(), m_results.end(),
                    std::back_inserter( s_searchByIdx ), [ idx ]( TestResult i )
                    {  return (i.m_testID == idx);} );
      for( const auto &result : s_searchByIdx )
      {
         std::cout << std::setw( 15 ) << left
               << VehicleRepoTest::m_testSequences[result.m_testID].testName
               << std::setw( 30 ) << left << result.m_repoName
               << std::setw( 10 ) << right << result.m_result << "usec"
               << std::endl;
      }
      std::cout << std::endl;
   }
}

void VehicleRepoTest::printRandomVehicleVin() const
{
   uint32_t idx = rand() % m_vehicles.size();
   std::string vin = m_vehicles[idx].getVin();
   std::cout << vin << std::endl;
}

void VehicleRepoTest::searchByYear()
{
   // search by Year
   cout << "\n\n\n" << endl;
   cout << " Task 5: Search for vehicles that match by year("
         << Vehicle::LowestYear << "-" << Vehicle::HighestYear << ") ......\n"
         << endl;
   uint16_t year = 0;

   while( true )
   {
      year = 0;
      string tmp;
      cout << "enter year:" << endl;
      cin >> tmp;
      if( !Vehicle::isValidYear( tmp, &year ) )
      {
         cerr << "invalid year" << endl;
      }
      else
      {
         for( const auto &repo : m_repoList )
         {
            // add data for searchVin to results
            std::string resultName(
                  VehicleRepoTest::m_testSequences[VehicleRepoTest::SEARCH_YEAR].testName );
            resultName.append( repo->getName() );

            Vehicle::clearCounts();
            std::chrono::system_clock::time_point before =
                  std::chrono::system_clock::now();
            size_t numFound = repo->searchByYear( year );
            std::chrono::system_clock::time_point after =
                  std::chrono::system_clock::now();
            uint64_t duration = (std::chrono::duration_cast<
                  std::chrono::microseconds>)( after - before ).count();

            cout << resultName << " found " << numFound << " vehicles for year:"
                  << year << " took " << duration << "us. \n";

            Vehicle::printCounts( std::cout );
            Vehicle::clearCounts();

            TestResult result( VehicleRepoTest::SEARCH_YEAR,
                               std::string( repo->getName() ), duration );
            m_results.push_back( result );

            cout << std::endl;
         }
      }
      std::string isContinue;
      cout << " continue searching? enter y to continue:";
      cin >> isContinue;
      if( isContinue != "y" ) break;
   }
}
;

void VehicleRepoTest::searchByModel()
{
   // search by model
   cout << "\n\n\n" << endl;
   cout << " Task 6: Search for vehicles that match by model: ......" << endl;
   Vehicle::e_Model model = Vehicle::e_Model::e_lastModel;
   while( true )
   {
      cout << "enter model index:1-" << Vehicle::e_Model::e_lastModel << ":"
            << endl;
      uint16_t choice = 0;
      for( auto item : Vehicle::m_modelMap )
      {
         cout << (item.index + 1) << " " << item.name << endl;
      }
      cin >> choice;
      if( (choice < 1) || (choice > Vehicle::e_Model::e_lastModel) )
      {
         cerr << "invalid choice:" << choice << endl;
      }
      else
      {
         model = (Vehicle::e_Model) (choice - 1);
         for( const auto &repo : m_repoList )
         {
            std::string resultName(
                  VehicleRepoTest::m_testSequences[VehicleRepoTest::SEARCH_MODEL].testName );
            resultName.append( repo->getName() );

            Vehicle::clearCounts();
            std::chrono::system_clock::time_point before =
                  std::chrono::system_clock::now();

            size_t numFound = repo->searchByModel( model );

            std::chrono::system_clock::time_point after =
                  std::chrono::system_clock::now();
            uint64_t duration = std::chrono::duration_cast<
                  std::chrono::microseconds>( after - before ).count();
            cout << resultName << " found " << numFound
                  << " vehicles for model:" << Vehicle::m_modelMap[model].name
                  << " took " << duration << "us. \n";

            Vehicle::printCounts( std::cout );
            cout << std::endl;

            TestResult result( VehicleRepoTest::SEARCH_MODEL,
                               std::string( repo->getName() ), duration );
            m_results.push_back( result );

         }
      }
      cout << "\n" << endl;
      std::string isContinue;
      cout << " continue searching? enter y to continue:";
      cin >> isContinue;
      if( isContinue != "y" ) break;
   }
}
;

void VehicleRepoTest::searchByVin()
{
   // search by model
   std::string vin;
   cout << "\n\n\n" << endl;
   cout << " Task 4: Search for vehicles that match by VIN......" << endl;
   while( true )
   {
      std::cout << "Suggestions:" << std::endl;
      for( int k = 0; k < 3; k++ )
      {
         printRandomVehicleVin();
      }
      cout << "\nenter Vin" << endl;
      cin >> vin;
      cout << vin << "\n" << endl;
      for( const auto &repo : m_repoList )
      {

         std::string resultName(
               VehicleRepoTest::m_testSequences[VehicleRepoTest::SEARCH_VIN].testName );
         resultName.append( repo->getName() );

         Vehicle::clearCounts();

         std::chrono::system_clock::time_point before =
               std::chrono::system_clock::now();
         bool found = repo->searchByVin( vin );
         std::chrono::system_clock::time_point after =
               std::chrono::system_clock::now();
         uint64_t duration = std::chrono::duration_cast<
               std::chrono::microseconds>( after - before ).count();

         if( found )
         {
            cout << resultName << " found vehicle with Vin:" << vin << " took "
                  << duration << "us.\n" << std::endl;
         }
         else
         {
            cout << resultName << " did NOT find vehicle with Vin:" << vin
                  << " took " << duration << "us. \n";
         }

         Vehicle::printCounts( std::cout );
         cout << std::endl;

         TestResult result( VehicleRepoTest::SEARCH_VIN,
                            std::string( repo->getName() ), duration );
         m_results.push_back( result );

      }
      cout << vin << "\n" << endl;
      std::string isContinue;
      cout << " continue searching? enter y to continue:";
      cin >> isContinue;
      if( isContinue != "y" ) break;
   }
}

void VehicleRepoTest::traverse()
{
   cout << "\n\n" << endl;
   cout << " Task 2: Traverse repositories...." << endl;
   for( const auto &repo : m_repoList )
   {
      std::string resultName(
            VehicleRepoTest::m_testSequences[VehicleRepoTest::TRAVERSE].testName );
      resultName.append( repo->getName() );

      Vehicle::clearCounts();

      std::chrono::system_clock::time_point before =
            std::chrono::system_clock::now();
      repo->traverse();
      std::chrono::system_clock::time_point after =
            std::chrono::system_clock::now();
      uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(
            after - before ).count();

      cout << resultName << " took " << duration << "us.\n";

      Vehicle::printCounts( std::cout );
      cout << std::endl;

      TestResult result( VehicleRepoTest::TRAVERSE,
                         std::string( repo->getName() ), duration );
      m_results.push_back( result );
   }
   cout << "\n" << endl;
}

void VehicleRepoTest::traverseAndWrite()
{
   cout << "\n\n" << endl;
   cout << " Task 3: Traverse and Write repositories...." << endl;
   for( const auto &repo : m_repoList )
   {
      std::string resultName(
            VehicleRepoTest::m_testSequences[VehicleRepoTest::TRAVERSE].testName );
      resultName.append( repo->getName() );

      std::chrono::system_clock::time_point before =
            std::chrono::system_clock::now();
      repo->traverseAndWrite();
      std::chrono::system_clock::time_point after =
            std::chrono::system_clock::now();
      uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(
            after - before ).count();

      cout << resultName << " took " << duration << "us.\n";

      Vehicle::printCounts( std::cout );
      cout << std::endl;

      TestResult result( VehicleRepoTest::TRAVERSE_WRITE,
                         std::string( repo->getName() ), duration );
      m_results.push_back( result );

   }
   cout << "\n" << endl;
}

