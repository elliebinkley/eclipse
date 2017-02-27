/*
 * Tries.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: larry burley
 *
 *   Description:   Tries: This puzzle is from  Gayle Laakmann McDowell, author of the best-selling interview book
 *      "Cracking the Coding Interview."
 *  We're going to make our own Contacts application! The application must perform two types of operations:
 *  1. add name, where name is a string denoting a contact name.  This must store  as a new contact in the application.
 *  2. find partial, where name is a string denoting a partial name to search the application for.
 *     It must count the number of contacts starting with name and print the count on a new line.
 *     Given  sequential add and find operations, perform each operation in order.
 *     Input Format
 *     The first line contains a single integer, , denoting the number of operations to perform.
 *     Each line  of the  subsequent lines contains an operation in one of the two forms defined above.
 *  Constraints:
 *         It is guaranteed that  and  contain lowercase English letters only.
 *         The input doesn't have any duplicates  for the  operation.
 *  Output Format
 *     For each find partial operation, print the number of contact names starting with  on a new line.
 *
 *  Sample Input
 *      4
 *      add hack
 *      add hackerrank
 *      find hac
 *      find hak
 *  Sample Output
 *      2
 *      0
 *  Explanation:
 *       We perform the following sequence of operations:
 *       Add a contact named hack.
 *       Add a contact named hackerrank.
 *       Find and print the number of contact names beginning with hac. There are currently two contact names
 *       in the application and both of them start with hac, so we print 2 on a new line.
 *       Find and print the number of contact names beginning with hak. There are currently two contact names
 *       in the application but neither of them start with hak, so we print 0 on a new line.
 *  Submissions: 8624  2/26/2017
 *       Max Score: 50
 *       Difficulty: Hard
 *  Solution: I coded this myself and it passed the inout set tests on the hackerrank site.
 *      Create a "TRIES" data structure that has a root node and up to 26 nodes underneath it; one for each
 *      letter. A node only exists if a name starts with that letter. E.g. for hack, h would be under the
 *      root  node. Under the node h, another node for 'a' would exist.  Under the node 'a', a node 'c' would
 *      exist. Under the node 'c', the node 'k' would exist. So only 4 nodes would exist.
 *      If we add the word "hat", then under the existing node, 'a', we would add the node 't'.
 *
 *      As an optimization for this puzzle, every node will have the total count of children nodes.
 *      So the node 'h' would have  a child count of 4. This is an optimization, since when finding partials,
 *      matches, we do not have to traverse the subtree to count the children.  Without this optimization, the
 *      program timed out on the HackerRank web site.
 *
 *      Tries are a good data structure for dictionary type of problems and probably for things like auto-spell
 *      correction or type ahead anticipation.
 *
 *      The code below does not handle deletes; it was not part of the original requirements.
 *
 */

#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <cassert>
#include <sstream>
#include "Utilities.hpp"

using namespace std;

bool
checkContact(const string& contact)
{
    for( auto &c : contact )
    {
        if( !isalpha(c) || ( !islower(c)) ) return false;
    }
    return true;
}

class Node
{
    public:
        Node() : m_isRoot(true), m_key('?')  {};
        Node(char letter ): m_isRoot(false), m_key(letter) {};
        int countNumChildLeaves();
        Node* add(const string& contact);
        Node* find(const string& contact);
        void print();
        virtual ~Node();

        int   m_numChildLeaves = 0;
        bool  m_isLeaf = false;
        bool  m_isRoot;
        char  m_key;
        map<char, Node*> m_map = {};
};

Node::~Node()
{
    for(auto &x : m_map )
    {
        if( x.second ) delete x.second;
    }
}

void
Node::print()
{
    cout << " Node:char=" << m_key << " isLeaf=" << m_isLeaf << " isRoot=" << m_isRoot
         << " m_numChildLeaves=" << m_numChildLeaves << endl;
    for( auto &node :  m_map )
    {
         node.second->print();
    }
};


// returns the node that was found to be the full or partial match for the string.
// returns nullptr if not found.
Node*
Node::find(const string& contact)
{
    Node* node = nullptr;                  // implies not found
    if(contact.size() == 0) return this;   // found the node.

    auto x = m_map.find(contact[0]);
    if(x != m_map.end())     //  found
    {
        string substring = contact.substr(1,contact.size());
        node = x->second->find( substring );
    }
    return node;
};

// return node that is added; return nullptr for duplicates.
Node*
Node::add(const string& contact )
{
    Node* node = nullptr;                  // implies duplicate
    if(contact.size() == 0)
    {
        m_isLeaf = true;
        node =  this;
    }
    else
    {
        string substring = contact.substr(1,contact.size());

        auto x = m_map.find(contact[0]);
        if(x == m_map.end())  // letter not found; add it
        {
            Node* addedNode = new Node( contact[0] );
            m_map[contact[0]] = addedNode;
            node = addedNode->add(substring);
            m_numChildLeaves++;
        }
        else // letter found; call it with remaining substring
        {
            node = x->second->add(substring);
            if ( node ) m_numChildLeaves++;
        }
    }
    return node;
};

// find n contacts partial or full matches
/* Enter your code here. Read input from STDIN. Print output to STDOUT */
void
triesPuzzle()
{
    T_START
    Node* root = new Node();

    // get input from file TriesInput_1.txt"
    int n = 0;
    std::string fileInput = "../TestData/TriesInput_1.txt";
    ifstream dataOperations( fileInput );
    if( !dataOperations.is_open() )
    {
        cout << "could not open "  << fileInput << endl;
    }
    std::string inputString;
    // first line should be an positive integer
    std::getline( dataOperations, inputString);
    try
    {
        n = std::stoi( inputString, nullptr );
    } catch (std::exception &e )
    {
        cout << e.what();
        cout << fileInput << ":bad input received=" + inputString << endl;
        assert( false );
        return;
    }

    // get expected output from find operations from file TriesExpectedOutput_1.txt
    std::string fileExpResults = "../TestData/TriesExpectedOutput_1.txt";
    ifstream strFileResults( fileExpResults );
    if( !strFileResults.is_open() )
    {
        cout << "could not open "  << fileExpResults << endl;
    }

    std::getline( strFileResults, inputString);
    // check if first line is correct
    if( inputString.find("expected output:") == std::string::npos)
    {
        cout << fileExpResults << ":bad input received=" + inputString << endl;
        assert( false );
        return;
    }

    // execute n operations; either a "add contact" or a "find contact" operation
    int numFindOps = 0;
    int numAddOps = 0;
    int numFindMatches = 0;
    for( int i=0; i<n; i++)
    {
        string operation;
        string contact;

        std::getline( dataOperations, inputString);
        stringstream ss(inputString);
        ss >> operation >> contact;
 //     cout << "operation=" << operation << endl;
 //     cout << "contact=" << contact << endl;

        if( !checkContact(contact) ) cout << "invalid contact" <<  contact << endl;

        if( operation == "add")
        {
            root->add(contact);
            numAddOps++;
        }
        else if( operation == "find")
        {
            Node* foundNode = root->find(contact);
            numFindOps++;

            int numFinds = 0;
            if(foundNode )
            {
                numFinds = (foundNode->m_numChildLeaves + (foundNode->m_isLeaf ? 1 : 0));
            }
            // get number expected for this find...
            int n = 0;
            std::getline( strFileResults, inputString);
            try
            {
                n = std::stoi( inputString, nullptr );
            } catch (std::exception &e )
            {
                cout << e.what();
                cout << fileInput << ":bad input received=" + inputString << endl;
                assert( false );
                return;
            }
            if( n != numFinds )
            {
               cout << "expected=" << n << " != " <<  numFinds << endl;
               assert( false );
               return;
            }
            else
            {
                numFindMatches++;
            }
         }
         else
         {
            cout << "invalid operation" <<   operation << endl;
         }
        /*
         cout << "\n tree is " << endl;
         root->print();
         cout << " end tree \n\n " << endl;
         */
    }

    cout << "SUCCESS: adds=" << numAddOps << " finds=" << numFindOps << " matches=" << numFindMatches << endl;
    delete root;
    T_END
    return;
}








