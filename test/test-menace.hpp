
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// test-menace.hpp                                                                                                                       //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Test file for MENACE helper functions


#include <menace.hpp>

namespace menaceTesting
{
using namespace menace;

BOOST_AUTO_TEST_SUITE(test_menace)

  // the following tests of operator overloads will use ITERATION only, as all enums will use these overloads the same

  // test increment operator overloads
  BOOST_AUTO_TEST_CASE(increment)
  {
    ITERATION itr=ITERATION::kZero;
    ++itr;
    // check itr has increased by one
    BOOST_CHECK(itr==ITERATION::kOne);
    itr++;
    // check itr has increased by one
    BOOST_CHECK(itr==ITERATION::kTwo);
  }

  // test equality operator overloads
  BOOST_AUTO_TEST_CASE(equality)
  {
    ITERATION itr=ITERATION::kFive;
    // check equality to an in
    BOOST_CHECK(itr==5);
    BOOST_CHECK(!(itr==4));
    // check inequality to an int
    BOOST_CHECK(itr!=4);
    BOOST_CHECK(!(itr!=5));
    // check inequality to another enum
    BOOST_CHECK(itr!=ITERATION::kSeven);
    BOOST_CHECK(!(itr!=ITERATION::kFive));
  }

  // test ordering operator overloads
  BOOST_AUTO_TEST_CASE(ordering)
  {
    ITERATION itr=ITERATION::kFive;
    // check less than or equal to another enum
    BOOST_CHECK(itr<=ITERATION::kFive);
    BOOST_CHECK(!(itr<=ITERATION::kFour));
    // check strictly less than an int
    BOOST_CHECK(itr<6);
    BOOST_CHECK(!(itr<4));
    // check strictly greater than an int
    BOOST_CHECK(itr>4);
    BOOST_CHECK(!(itr>6));
  }

  // test modulo operator overloads
  BOOST_AUTO_TEST_CASE(modulo)
  {
    ITERATION itr=ITERATION::kFive;
    // check modulo an in
    BOOST_CHECK(itr%5==0);
    BOOST_CHECK(!(itr%5==1));
    BOOST_CHECK(itr%4==1);
    BOOST_CHECK(!(itr%4==2));
  }

  // test addition operator overloads
  BOOST_AUTO_TEST_CASE(addition)
  {
    ITERATION itr=ITERATION::kFive;
    // check adding an int
    BOOST_CHECK(itr+1==ITERATION::kSix);
    BOOST_CHECK(!(itr+1==ITERATION::kSeven));
    itr+=1;
    // check addition assignment
    BOOST_CHECK(itr==ITERATION::kSix);
    BOOST_CHECK(!(itr==ITERATION::kSeven));
  }

  // test subration operator overloads
  BOOST_AUTO_TEST_CASE(subtraction)
  {
    ITERATION itr=ITERATION::kFive;
    // check subtracting an int
    BOOST_CHECK(itr-1==ITERATION::kFour);
    BOOST_CHECK(!(itr-1==ITERATION::kSeven));
  }

  // test multiplication operator overloads
  BOOST_AUTO_TEST_CASE(multiplication)
  {
    ITERATION itr=ITERATION::kThree;
    // check multiplying by an int
    BOOST_CHECK(itr*0==ITERATION::kZero);
    BOOST_CHECK(itr*1==ITERATION::kThree);
    BOOST_CHECK(itr*2==ITERATION::kSix);
    BOOST_CHECK(!(itr*0==ITERATION::kOne));
    BOOST_CHECK(!(itr*1==ITERATION::kOne));
    BOOST_CHECK(!(itr*2==ITERATION::kOne));
  }

  // test division operator overloads
  BOOST_AUTO_TEST_CASE(division)
  {
    ITERATION itr=ITERATION::kSix;
    // check dividing by an int
    BOOST_CHECK(itr/1==ITERATION::kSix);
    BOOST_CHECK(itr/2==ITERATION::kThree);
    BOOST_CHECK(!(itr/1==ITERATION::kOne));
    BOOST_CHECK(!(itr/2==ITERATION::kOne));
  }

  // test inline functions
  BOOST_AUTO_TEST_CASE(inlines)
  {
    std::vector<int> int_vec_one={1,2,3},int_vec_two={4,5,6};
    std::vector<int> int_sum=int_vec_one+int_vec_two;
    for (int itr=1;itr<7;++itr)
    {
      // check the vectors summed correctly
      BOOST_CHECK(int_sum[itr-1]==itr);
    }

    std::vector<char> char_vec_one={'a','b','c'},char_vec_two={'d','e','f'};
    std::vector<char> char_sum=char_vec_one+char_vec_two;
    for (int itr=0;itr<6;++itr)
    {
      // check the vectors summed correctly
      BOOST_CHECK(char_sum[itr]==static_cast<char>(itr+97));
    }

    // check values are found at the right location
    BOOST_CHECK(find(int_sum,1)==0);
    BOOST_CHECK(find(int_sum,3)==2);
    BOOST_CHECK(find(int_sum,6)==5);
    // check values not found return -1
    BOOST_CHECK(find(int_sum,0)==-1);
    BOOST_CHECK(find(int_sum,7)==-1);

    // check values are found at the right location
    BOOST_CHECK(find(char_sum,'a')==0);
    BOOST_CHECK(find(char_sum,'c')==2);
    BOOST_CHECK(find(char_sum,'f')==5);
    // check values not found return -1
    BOOST_CHECK(find(char_sum,'g')==-1);
    BOOST_CHECK(find(char_sum,'A')==-1);
  }

  // test constants of the project
  BOOST_AUTO_TEST_CASE(constants)
  {
    // check kUnhelpableCommands
    BOOST_CHECK(kUnhelpableCommands.at(0)=="help");
    BOOST_CHECK(kUnhelpableCommands.at(1)=="cls");
    BOOST_CHECK(kUnhelpableCommands.at(2)=="exit");

    // check kPositions
    BOOST_CHECK(kPositions.at(0)=="topleft");
    BOOST_CHECK(kPositions.at(1)=="topmiddle");
    BOOST_CHECK(kPositions.at(2)=="topright");
    BOOST_CHECK(kPositions.at(3)=="middleleft");
    BOOST_CHECK(kPositions.at(4)=="middlemiddle");
    BOOST_CHECK(kPositions.at(5)=="middleright");
    BOOST_CHECK(kPositions.at(6)=="bottomleft");
    BOOST_CHECK(kPositions.at(7)=="bottommiddle");
    BOOST_CHECK(kPositions.at(8)=="bottomright");

    // check kRowNames
    BOOST_CHECK(kRowNames.at(0)=="top");
    BOOST_CHECK(kRowNames.at(1)=="middle");
    BOOST_CHECK(kRowNames.at(2)=="bottom");

    // check kGameExitCommands
    BOOST_CHECK(kGameExitCommands.at(0)=="quit");
    BOOST_CHECK(kGameExitCommands.at(1)=="q");
    BOOST_CHECK(kGameExitCommands.at(2)=="break");
    BOOST_CHECK(kGameExitCommands.at(3)=="exit");

    // check kColours
    BOOST_CHECK(kColours.at(COLOUR::kRed)=="\033[1;31m");
    BOOST_CHECK(kColours.at(COLOUR::kGreen)=="\033[1;32m");
    BOOST_CHECK(kColours.at(COLOUR::kYellow)=="\033[1;33m");
    BOOST_CHECK(kColours.at(COLOUR::kBlue)=="\033[1;34m");
    BOOST_CHECK(kColours.at(COLOUR::kPurple)=="\033[1;35m");
    BOOST_CHECK(kColours.at(COLOUR::kCyan)=="\033[1;36m");

    // check kColourEnd
    BOOST_CHECK(kColourEnd=="\033[0m");

    // check kUserRequests
    BOOST_CHECK(kUserRequests.at("advise")==REQUEST::kAdvise);
    BOOST_CHECK(kUserRequests.at("move")==REQUEST::kMove);

    // check kValueStrings
    BOOST_CHECK(kValueStrings.at(VALUE::kEmpty)==" ");
    BOOST_CHECK(kValueStrings.at(VALUE::kCross)=="X");
    BOOST_CHECK(kValueStrings.at(VALUE::kNought)=="O");

    // check kValueColours
    BOOST_CHECK(kValueColours.at(VALUE::kEmpty)==COLOUR::kRed);
    BOOST_CHECK(kValueColours.at(VALUE::kCross)==COLOUR::kPurple);
    BOOST_CHECK(kValueColours.at(VALUE::kNought)==COLOUR::kBlue);

    // check kPositionStrings
    BOOST_CHECK(kPositionStrings.at(POSITION::kTopLeft)=="top left");
    BOOST_CHECK(kPositionStrings.at(POSITION::kTopMiddle)=="top middle");
    BOOST_CHECK(kPositionStrings.at(POSITION::kTopRight)=="top right");
    BOOST_CHECK(kPositionStrings.at(POSITION::kMiddleLeft)=="middle left");
    BOOST_CHECK(kPositionStrings.at(POSITION::kMiddleMiddle)=="middle middle");
    BOOST_CHECK(kPositionStrings.at(POSITION::kMiddleRight)=="middle right");
    BOOST_CHECK(kPositionStrings.at(POSITION::kBottomLeft)=="bottom left");
    BOOST_CHECK(kPositionStrings.at(POSITION::kBottomMiddle)=="bottom middle");
    BOOST_CHECK(kPositionStrings.at(POSITION::kBottomRight)=="bottom right");

    // check kHumanIncludedGames
    BOOST_CHECK(kHumanIncludedGames.at(0)==GAME::kPvP);
    BOOST_CHECK(kHumanIncludedGames.at(1)==GAME::kPvR);
    BOOST_CHECK(kHumanIncludedGames.at(2)==GAME::kPvM);
    BOOST_CHECK(kHumanIncludedGames.at(3)==GAME::kMvP);
    BOOST_CHECK(kHumanIncludedGames.at(4)==GAME::kRvP);

    // check kHumanFirstGamess
    BOOST_CHECK(kHumanFirstGames.at(0)==GAME::kPvP);
    BOOST_CHECK(kHumanFirstGames.at(1)==GAME::kPvR);
    BOOST_CHECK(kHumanFirstGames.at(2)==GAME::kPvM);

    // check kAutomatonFirstGames
    BOOST_CHECK(kAutomatonFirstGames.at(0)==GAME::kMvM);
    BOOST_CHECK(kAutomatonFirstGames.at(1)==GAME::kMvP);
    BOOST_CHECK(kAutomatonFirstGames.at(2)==GAME::kMvR);
    BOOST_CHECK(kAutomatonFirstGames.at(3)==GAME::kRvR);
    BOOST_CHECK(kAutomatonFirstGames.at(4)==GAME::kRvP);
    BOOST_CHECK(kAutomatonFirstGames.at(5)==GAME::kRvM);

    // check kMENACEFirstGames
    BOOST_CHECK(kMENACEFirstGames.at(0)==GAME::kMvM);
    BOOST_CHECK(kMENACEFirstGames.at(1)==GAME::kMvP);
    BOOST_CHECK(kMENACEFirstGames.at(2)==GAME::kMvR);

    // check kRandomFirstGames
    BOOST_CHECK(kRandomFirstGames.at(0)==GAME::kRvR);
    BOOST_CHECK(kRandomFirstGames.at(1)==GAME::kRvP);
    BOOST_CHECK(kRandomFirstGames.at(2)==GAME::kRvM);

    // check kHumanSecondGames
    BOOST_CHECK(kHumanSecondGames.at(0)==GAME::kPvP);
    BOOST_CHECK(kHumanSecondGames.at(1)==GAME::kMvP);
    BOOST_CHECK(kHumanSecondGames.at(2)==GAME::kRvP);

    // check kAutomatonSecondGames
    BOOST_CHECK(kAutomatonSecondGames.at(0)==GAME::kMvM);
    BOOST_CHECK(kAutomatonSecondGames.at(1)==GAME::kPvM);
    BOOST_CHECK(kAutomatonSecondGames.at(2)==GAME::kRvM);
    BOOST_CHECK(kAutomatonSecondGames.at(3)==GAME::kRvR);
    BOOST_CHECK(kAutomatonSecondGames.at(4)==GAME::kPvR);
    BOOST_CHECK(kAutomatonSecondGames.at(5)==GAME::kMvR);

    // check kMENACESecondGames
    BOOST_CHECK(kMENACESecondGames.at(0)==GAME::kMvM);
    BOOST_CHECK(kMENACESecondGames.at(1)==GAME::kPvM);
    BOOST_CHECK(kMENACESecondGames.at(2)==GAME::kRvM);

    // check kRandomSecondGames
    BOOST_CHECK(kRandomSecondGames.at(0)==GAME::kRvR);
    BOOST_CHECK(kRandomSecondGames.at(1)==GAME::kPvR);
    BOOST_CHECK(kRandomSecondGames.at(2)==GAME::kMvR);
  }

BOOST_AUTO_TEST_SUITE_END()
}
