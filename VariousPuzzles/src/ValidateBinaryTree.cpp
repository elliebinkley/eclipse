/*
 * ValidateBinaryTree.cpp
 *
 *  Created on: Feb 5, 2019
 *      Author: USER
 *
 *  This exercise is taken from  "Cracking the Coding Interview". I solved it on the web, then took the solutions and put it in this
 * suite of solved puzzles. See https://www.hackerrank.com/domains/tutorials/cracking-the-coding-interview under login:lburley ;
 * Trees: Is This a Binary Search Tree?
 * Here I had to add infrastructure that was provided on the web, e.g. a AVL search tree with data.
 * The test set data is just a vector where the initial data inserted is just the vector index.
 * The data is inserted into the tree, the tree is validated, then the data in the tree is modifed to make the tree invalid, then
 * the tree is validated, which finds the errors.    The data is made invalid according to the values in the vector below, where some of the data
 * values do not match the index, e.g. vector v3 has the numbers 17 and 16 switched. The test code finds a Node with 16 in the tree and changes it to 17.
 * The the validate() is run on the tree which should find the error.
 *
 * From Hackerrank:
 *  Validate a binary tree.
 *  For the purposes of this challenge, we define a binary search tree to be a binary tree with the following ordering properties:
 *      The  value of every node in a node's left subtree is less than the data value of that node.
 *      The  value of every node in a node's right subtree is greater than the data value of that node.
 *      Given the root node of a binary tree, can you determine if it's also a binary search tree?
 *      Complete the function in your editor below, which has  parameter: a pointer to the root of a binary tree.
 *      It must return a boolean denoting whether or not the binary tree is a binary search tree.
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include "Utilities.hpp"

using namespace std;
// input data taken from hackerrank...
static const bool isDataSetValid0 = true;
static vector v0 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
      22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,
      39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,
      75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,
      109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,
      136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,
      163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,
      190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,
      217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,
      244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,
      271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,
      298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,
      325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,
      352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,
      378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,
      405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,
      432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,
      459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,
      486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,
      513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,
      540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,
      567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,
      594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,
      621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,
      648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,
      675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,
      702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,
      729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,
      756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,
      783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,
      810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,
      837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,
      864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,
      891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,
      918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,940,941,942,943,944,
      945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,963,964,965,966,967,968,969,970,971,
      972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,
      999,1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,
      1021,1022,1023};

static const bool isDataSetValid1 = false;
static vector v1={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
      52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,
      100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,
      136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,
      173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,
      209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,
      245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,
      281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,
      318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,
      354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,
      390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,
      426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,
      463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,
      499,500,501,504,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,
      535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,
      571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,
      608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,
      644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,
      680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,
      716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,
      753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,
      789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,
      825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,
      862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,
      898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,
      934,935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,963,964,965,966,967,968,969,
      970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999,1000,1001,1002,1003,1004,
      1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023};

static const bool isDataSetValid2 = false;
static vector v2 =  { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
      46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,
      93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,
      130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,
      165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,
      200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,
      270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,
      305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,
      340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,
      375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,
      410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,
      445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,
      480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,
      515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,
      550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,
      585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,
      620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,
      655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,
      690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,
      725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,758,759,
      760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,789,790,791,792,793,794,
      795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,
      830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,
      865,866,867,868,869,870,871,872,873,874,875,876,875,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,
      900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,
      935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,963,964,965,966,967,968,
      969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999,1000,1001,1002,
      1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023};

static const bool isDataSetValid3 = false;
static vector v3 ={ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };

class TreeTester;

class AVL
{
   friend TreeTester;
public:
   AVL( int data, int index ) {  m_root = new Element( data, index ); }
   ~AVL()       { if ( m_root != nullptr ) delete m_root; }
   inline void add( int data, int index  )  { m_root->addItem( data, index, &m_root  ); }
   inline bool validate() { return( m_root->validate() ); }
   void print();
private:
   class Element
   {
      friend TreeTester;
   public:
      Element(int data, int index) : m_data(data), m_index(index), m_left(nullptr), m_right(nullptr) {}
      ~Element();
      bool addItem( int data, int index, Element** parent  );
      inline int getData() { return m_data; }
      inline int getIndex() { return m_index; }
      void print( std::stringstream& ss );
      bool validate();
   private:
      int getDepth();
      int getLeftDepth();
      int getRightDepth();
      int getHighest();
      int getLowest();
      void rebalanceToTheLeft(Element** parentLink );
      void rebalanceToTheRight(Element** parentLink );

      int m_data;       // Node value
      int m_index;      // Node is added to the tree with this comparison value
      Element* m_left;
      Element* m_right;
   };
   Element* m_root;
};

class TreeTester
{
public:
   // make the m_index match the m_data, thus messing up the
   // AVL tree integrity.  Then run audit to find errors.
   void validateAVLTree();
private:
   void messUpAVL();
   void changeIndex( AVL::Element* node, stringstream& ss );
   AVL* m_avl = 0;
};

// function added to run in this test suite.
void validatekBinaryTree()
{
   T_START
   TreeTester t;
   t.validateAVLTree();
   T_END
}

// AVL definitions.

int AVL::Element::getLeftDepth()
{
  if( m_left != nullptr) return( m_left->getDepth() );
  return 0;
}

int AVL::Element::getRightDepth()
{
  if( m_right != nullptr) return( m_right->getDepth() );
  return 0;
}

// adds and rebalances the tree if needed.
bool AVL::Element::addItem( int data, int index, Element** parent  )
{
  bool ret = true;
  int diff =  0;

  if( index == m_index ) return false; // dups are discarded
  if( index < m_index )
  {
      if( m_left == nullptr )  m_left = new Element(data, index);
      else  ret = m_left->addItem( data, index, &m_left  );
  }
  if( index > m_index )
  {
      if( m_right == nullptr )  m_right = new Element(data,index);
      else  ret = m_right->addItem( data,index, &m_right );
  }

  if( ret )
  {
    diff =  getLeftDepth() - getRightDepth();
    if( diff > 1 )  rebalanceToTheRight(parent);
    if( diff < -1 ) rebalanceToTheLeft(parent);
  }
  return ( ret );
}

int AVL::Element::getDepth()
{
  int leftDepth = getLeftDepth();
  int rightDepth = getRightDepth();
  return ( leftDepth > rightDepth )? leftDepth + 1: rightDepth + 1;
}

void AVL::Element::rebalanceToTheLeft(Element** parentLink )
{
  Element* child = m_right;
  m_right = child->m_left;
  child->m_left = this;
  *parentLink = child;
}

void AVL::Element::rebalanceToTheRight(Element** parentLink )
{
  Element* child = m_left;
  m_left = child->m_right;   // set left to by child's right.
  child->m_right = this;     // set child right to be this
  *parentLink = child;       // set this's parent to point to child
}

// print info about this node
void AVL::Element::print( std::stringstream& ss )
{
  int left = 0;
  int leftDepth = getLeftDepth();
  int right = 0;
  int rightDepth = getRightDepth();
  if( m_left != nullptr) left = m_left->m_index;
  if( m_right != nullptr) right = m_right->m_index;

  //print left
  if( m_left != nullptr) m_left->print(ss);

  //print this
  ss << m_data << "," << m_index << " l:" << left << " r:" << right
     << " depth=" << getDepth()
     << " lDepth=" << leftDepth
     << " rDepth=" << rightDepth;
  if( abs( leftDepth - rightDepth) > 1 ) ss << " ??";
  ss << "\n";

  // print right
  if( m_right != nullptr) m_right->print(ss);

  return;
}

// audit this node
bool AVL::Element::validate()
{
   stringstream ss;
   bool ret = true;
   // check if m_index is greater than left subtree
   if ( m_left )
   {
      if (! (ret = ( m_index > m_left->getHighest() )) )
      {
         ss << "failed left validate():" << m_index << endl;
         print(ss);
      }
      else ret = m_left->validate();
   }
   // check if m_index is less than right
   if ( ret && m_right )
   {

      if ( !(ret = ( m_index < m_right->getLowest() )) )
      {
         ss << "failed right validate():" << m_index << endl;
         print(ss);
      }
      else ret = m_right->validate();
   }
   // Check if depth of sub trees are within 1 as per AVL definition
   int l_depth =  getLeftDepth();
   int r_depth =  getRightDepth();

   if( abs( l_depth - r_depth ) > 1 )
   {
      ret = false;
      ss << "depth mismatch: leftDepth="<< l_depth << " rightDepth=" << r_depth << endl;
   }

   if ( !ret ) cout << ss.str() << endl;
   return ret;
}

int AVL::Element::getHighest()
{
   int highest = m_index;
   if ( m_left )
   {
      int l_highest = m_left->getHighest();
      highest = ( m_index >  l_highest ) ? m_index : l_highest;
   }
   if ( m_right )
   {
      int r_highest = m_right->getHighest();
      highest = ( highest > r_highest ) ? highest : r_highest;
   }
   return highest;
}

int AVL::Element::getLowest()
{
   int lowest = m_index;
   if ( m_left )
   {
      int l_lowest = m_left->getLowest();
      lowest = ( m_index < l_lowest) ? m_index : l_lowest;
   }
   if ( m_right )
   {
      int r_lowest= m_right->getLowest();
      lowest= ( lowest < r_lowest) ? lowest: r_lowest;
   }
   return lowest;
}

AVL::Element::~Element()
{
   if ( m_left != nullptr )  delete m_left;
   if ( m_right != nullptr ) delete m_right;
}

void AVL::print()
{
  std::stringstream ss;
  m_root->print(ss);
  std::cout << ss.str() << std::endl;
  std::cout << "root=" << m_root->getData() << "," << m_root->getIndex() << std::endl;
}

void TreeTester::messUpAVL()
{
   AVL::Element* node = m_avl->m_root;
   stringstream ss;
   changeIndex( node, ss );
}

void TreeTester::changeIndex( AVL::Element* node, stringstream& ss )
{
   if( node->m_index != node->m_data )
   {
      ss << "Changing index=" << node->m_index << " to " << node->m_data << endl;
      node->m_index = node->m_data;
   }
   if ( node->m_left != nullptr ) changeIndex( node->m_left, ss);
   if ( node->m_right!= nullptr ) changeIndex( node->m_right, ss);
}


// function added to run in this test suite.
void TreeTester::validateAVLTree()
{
   T_START
   cout << "Validate that a AVL tree is a valid binary tree \n"
         << "  1. The  value of every node in a node's left subtree is less "
         << "     than the data value of that node.\n"
         << "  2. The  value of every node in a node's right subtree is"
         << "     greater than the data value of that node."
         << "  3. The AVL tree is populated with good data and validated,"
         << "     then the data is corrupted and rechecked"
         << endl;
   vector<vector<int>*> setOfDataSets = { &v0, &v1, &v2, &v3 };
   std::vector<bool> setOfDataSetsValid = { isDataSetValid0, isDataSetValid1, isDataSetValid2,
         isDataSetValid3 };
   for( unsigned int i = 0; i < setOfDataSets.size(); i++ )
   {
      vector<int>* v = setOfDataSets[i];
      AVL* avl = new AVL( (*v)[0], 0 );
      for( unsigned int i = 1; i < v->size(); i++ )
      {
         avl->add( (*v)[i], i );
      }

      // validate the initial tree is correct
      if ( avl->validate() ) cout << "SUCCESS: initial dataset[" << i << "]" << endl;
      else cout << "FAILURE: initial dataset[" << i << "]" << endl;

      // mess up tree
      stringstream ss;
      TreeTester::changeIndex( avl->m_root, ss );
      cout << ss.str() << endl;

      // detect corruptions
      if ( avl->validate() != setOfDataSetsValid[i] )
         cout << "FAILED: dataset[" << i << "]" << endl;
      else
         cout << "SUCCESS: dataset[" << i << "] \n\n" << endl;

      delete avl;
   }
   T_END
}

