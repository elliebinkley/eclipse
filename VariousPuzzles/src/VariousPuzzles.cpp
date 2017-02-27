/*
 * VariousPuzzles.cpp
 *
 *
 *
 *  Created on: Feb 22, 2017
 *      Author: USER
 */

#include "Utilities.hpp"
#include "iostream"
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

extern void homeMadeSort();
extern void reverseWords();
extern void createStairCase();
extern void inputOutputExample();
extern void bloombergStringTest();
extern void anagram();
extern void duplicateEntries();
extern void validatekBinaryTree();
extern void ransomNotePuzzle();
extern void triesPuzzle();


void badFunction()
{
    cout << "bad function" << endl;
}

const std::string desc1 = "\nWhat puzzles do you want to run? \n";

const std::string desc2 = "\nEnter in the puzzle numbers like so: 1 3 5 w\n"
        " when done enter any letter. e.g  w  ";

typedef enum PuzzleListEnum
{
    e_none = 0,
    e_homeMadeSort = 1,
    e_reverseWords = 2,
    e_createStairCase = 3,
    e_inputOutputExample = 4,
    e_bloombergStringTest = 5,
    e_anagram = 6,
    e_duplicateEntries = 7,
    e_validatekBinaryTree = 8,
    e_ransomNotePuzzle = 9,
    e_triesPuzzle = 10,
    e_last = 11
} puzzleEnum;

const typedef struct
{
    puzzleEnum index;
    const char* str;
    void (*funcptr)();
} Puzzle;

constexpr std::array<Puzzle, puzzleEnum::e_last> puzzleList =
{ {
        { PuzzleListEnum::e_none,"badEntry", badFunction },
        { PuzzleListEnum::e_homeMadeSort, "homeMadeSort", homeMadeSort },
        { PuzzleListEnum::e_reverseWords, "reverseWords", reverseWords },
        { PuzzleListEnum::e_createStairCase, "createStairCase", createStairCase },
        { PuzzleListEnum::e_inputOutputExample, "inputOutputExample", inputOutputExample },
        { PuzzleListEnum::e_bloombergStringTest, "bloombergStringTest",bloombergStringTest },
        { PuzzleListEnum::e_anagram, "anagram", anagram },
        { PuzzleListEnum::e_duplicateEntries, "duplicateEntries", duplicateEntries  },
        { PuzzleListEnum::e_validatekBinaryTree, "validatekBinaryTree", validatekBinaryTree  },
        { PuzzleListEnum::e_ransomNotePuzzle, "ransomNotePuzzle", ransomNotePuzzle  },
        { PuzzleListEnum::e_triesPuzzle, "triesPuzzle", triesPuzzle  }
} };

int main()
{
    T_START

    // present user with list of puzzles to run.
    std::cout << desc1 << endl;
    for( int i = 1; i < PuzzleListEnum::e_last; i++ )
    {
        Puzzle puzzle = puzzleList[i];
        cout << puzzle.index << ". " << puzzle.str << endl;
    }
    std::cout << desc2 << endl;

    // gather list of tests to run
    std::vector<int> testList;
    int testIndex;
    while( std::cin >> testIndex )
    {
        if( (testIndex <= PuzzleListEnum::e_none) || (testIndex >= PuzzleListEnum::e_last) )
        {
            std::cout << " bad input=" << testIndex;
            continue;
        }
        // check for duplicates
        if( std::find( testList.begin(), testList.end(), testIndex ) == testList.end() )
        {
            testList.push_back( testIndex );
        }
    }

    // cin must have a non-integer character still in it, since that's why
    // we broke out of the while loop above. Clear it out..
    std::cin.clear();    // clears error flag but  not the data...
   // cin.ignore(cin.rdbuf()->in_avail(), '\n');   // doesn't work in Cygwin..
    std::string s;
    cin >> s;

    // run list of tests
    for( auto &testIndex : testList )
    {
        try
        {
            std::cout << "START of Puzzle " << testIndex << " " << puzzleList[testIndex].str
                    << std::endl;
            ( *puzzleList[testIndex].funcptr)();
            std::cout << "END of Puzzle " << testIndex << " " << puzzleList[testIndex].str << "\n\n"
                    << std::endl;
        }
        catch( std::exception& e )
        {
            std::cout << "caught exception for Puzzle[" << testIndex << "]="
                    << puzzleList[testIndex].str << ":\n" << e.what() << " \n" << std::endl;
        }

    }
    T_END

    return 0;
}

