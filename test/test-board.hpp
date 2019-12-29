
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// test-board.hpp                                                                                                                        //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Test file for the Board class of MENACE


#include <array>
#include <board.hpp>

namespace BoardTesting
{
using namespace menace;

// the VALUE values for two distinct drawn boards to be copied from
const std::array<VALUE,9> draw_one={
  VALUE::kCross,VALUE::kNought,VALUE::kCross,VALUE::kCross,VALUE::kNought,VALUE::kCross,VALUE::kNought,VALUE::kCross,VALUE::kNought
},draw_two={
  VALUE::kNought,VALUE::kCross,VALUE::kCross,VALUE::kCross,VALUE::kNought,VALUE::kNought,VALUE::kCross,VALUE::kNought,VALUE::kCross
};

BOOST_AUTO_TEST_SUITE(test_board)

  // test initialising board objects
  BOOST_AUTO_TEST_CASE(initialisation)
  {
    // check initialising board never throws
    BOOST_CHECK_NO_THROW(Board());
    Board board;
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION access=pos;
      // check every position of the board is initialised to VALUE::kEmpty
      BOOST_CHECK(board.at(access)==VALUE::kEmpty);
      BOOST_CHECK(board[access]==VALUE::kEmpty);
    }
    // check plays is initialised to 0
    BOOST_CHECK(board.get_plays()==ITERATION::kZero);
    // check the board has result RESULT::kNotWon
    BOOST_CHECK(board.check_board()==RESULT::kNotWon);
  }

  // test setting positions on the board
  BOOST_AUTO_TEST_CASE(getters_and_setters)
  {
    // check setting any position to kEmpty throws an error
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      Board board;
      BOOST_CHECK_THROW(board.assign_position(pos,VALUE::kEmpty),assigning_empty_value);
    }

    // check every position can be set to kCross and kNought
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION access=pos;
      for (VALUE val=VALUE::kCross; val<=VALUE::kNought;++val)
      {
        Board board;
        BOOST_CHECK_NO_THROW(board.assign_position(pos,val));
        BOOST_CHECK(board.at(access)==val);
        BOOST_CHECK(board[access]==val);
      }
    }

    // test that every position can be set to kCross or kNought and an error is thrown when attempting to set it to the other
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION access=pos;
      for (VALUE val=VALUE::kCross; val<=VALUE::kNought;++val)
      {
        Board board;
        BOOST_CHECK_NO_THROW(board.assign_position(pos,val));
        BOOST_CHECK(board.at(access)==val);
        BOOST_CHECK(board[access]==val);
        VALUE other=val==VALUE::kNought ? VALUE::kCross : VALUE::kNought;
        BOOST_CHECK_THROW(board.assign_position(pos,other),non_empty_position);
        // check the error has been created correctly
        try
        {
          board.assign_position(pos,other);
        }
        catch (const non_empty_position &err)
        {
          BOOST_CHECK(err.pos==pos);
          BOOST_CHECK(err.val==val);
        }
      }
    }

    // test every position can be set to kCross and kNought and accessed from a Board and a const Board
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION access=pos;
      for (VALUE val=VALUE::kCross; val<=VALUE::kNought;++val)
      {
        Board board;
        BOOST_CHECK_NO_THROW(board.assign_position(pos,val));
        BOOST_CHECK(board.at(access)==val);
        BOOST_CHECK(board[access]==val);
        const Board const_board=board;
        BOOST_CHECK(const_board.at(access)==val);
        BOOST_CHECK(const_board[access]==val);
      }
    }

    // test playing when the board has been won throws an error and that the board can be checked for a winning board
    for (VALUE val=VALUE::kCross; val<=VALUE::kNought;++val)
    {
      VALUE other=val==VALUE::kCross ? VALUE::kNought : VALUE::kNought;
      // do the check for every winning combination
      for (const std::array<POSITION,3> &itr:kWinningPositions)
      {
        Board board;
        // place the winning positions
        for (POSITION pos:itr)
        {
          BOOST_CHECK_NO_THROW(board.assign_position(pos,val));
        }
        int cnt=0;
        // fill some other two, and check it throws on the third
        for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
        {
          const POSITION access=pos;
          // if we haven't yet placed two
          if (cnt<2)
          {
            // if the value is empty, play it
            if (board[access]==VALUE::kEmpty)
            {
              BOOST_CHECK_NO_THROW(board.assign_position(pos,other));
              ++cnt;
              continue;
            }
            // check throw if it's not empty
            else if (board[access]==val)
            {
              BOOST_CHECK_THROW(board.assign_position(pos,other),non_empty_position);
            }
          }
          // if we have placed two
          if (cnt==2)
          {
            // check throw if it's not empty
            if (board[access]!=VALUE::kEmpty)
            {
              BOOST_CHECK_THROW(board.assign_position(pos,other),non_empty_position);
              continue;
            }
            // else check throw that the game has been won (the real reason we are here)
            else { BOOST_CHECK_THROW(board.assign_position(pos,other),game_finished); }
            BOOST_CHECK(board.check_board()==static_cast<RESULT>(val));
            // check the error has been created correctly
            try
            {
              board.assign_position(pos,other);
            }
            catch (const game_finished &err)
            {
              BOOST_CHECK(err.result==static_cast<RESULT>(val));
            }
          }
        }
      }
    }

    // test bounds checking
    {
      Board board;
      const Board const_board;
      // below bounds
      POSITION pos=static_cast<POSITION>(-1);
      const POSITION access_one=pos;
      BOOST_CHECK_THROW(static_cast<void>(board.at(access_one)),std::out_of_range);
      BOOST_CHECK_THROW(static_cast<void>(board[access_one]),std::out_of_range);
      BOOST_CHECK_THROW(static_cast<void>(const_board.at(access_one)),std::out_of_range);
      BOOST_CHECK_THROW(static_cast<void>(const_board[access_one]),std::out_of_range);
      // above bounds
      pos=static_cast<POSITION>(9);
      const POSITION access_two=pos;
      BOOST_CHECK_THROW(static_cast<void>(board.at(access_two)),std::out_of_range);
      BOOST_CHECK_THROW(static_cast<void>(board[access_two]),std::out_of_range);
      BOOST_CHECK_THROW(static_cast<void>(const_board.at(access_two)),std::out_of_range);
      BOOST_CHECK_THROW(static_cast<void>(const_board[access_two]),std::out_of_range);
    }
  }

  // test assignment operator
  BOOST_AUTO_TEST_CASE(assignment)
  {
    // hard code a drawn board
    Board board;
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION loc=pos;
      board.assign_position(loc,draw_one.at(static_cast<int>(loc)));
    }
    // assign
    Board other=board;
    // test
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION loc=pos;
      BOOST_CHECK(other.at(loc)==draw_one[static_cast<int>(pos)]);
    }
  }

  // test clearing the board
  BOOST_AUTO_TEST_CASE(clear)
  {
    // hard code a drawn board
    Board board;
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION loc=pos;
      board.assign_position(loc,draw_one.at(static_cast<int>(loc)));
    }
    // clear
    board.clear_board();
    // test
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION access=pos;
      BOOST_CHECK(board[access]==VALUE::kEmpty);
    }
  }

  // test the board element
  BOOST_AUTO_TEST_CASE(board_element)
  {
    // test BoardElement default constructor
    BoardElement elem_one;
    BOOST_CHECK(elem_one.val==VALUE::kEmpty);
    BOOST_CHECK(elem_one.pos==0);
    BOOST_CHECK(elem_one.ind==0);

    // test BoardElement constructor
    BoardElement elem_two(VALUE::kCross,POSITION::kMiddleRight,5);
    BOOST_CHECK(elem_two.val==VALUE::kCross);
    BOOST_CHECK(elem_two.pos==POSITION::kMiddleRight);
    BOOST_CHECK(elem_two.ind==5);

    // test BoardElement constructor throws
    BOOST_CHECK_THROW(BoardElement(VALUE::kCross,static_cast<POSITION>(-1),-1),std::invalid_argument);
    BOOST_CHECK_THROW(BoardElement(VALUE::kCross,static_cast<POSITION>(10),10),std::invalid_argument);
    BOOST_CHECK_THROW(BoardElement(VALUE::kCross,static_cast<POSITION>(4),3),std::invalid_argument);

    // test the exceptions are properly configured
    BoardElement elem_three;
    try
    {
      elem_three=BoardElement(VALUE::kCross,static_cast<POSITION>(-1),-1);
    }
    catch(const std::invalid_argument &err)
    {
      BOOST_CHECK(std::string(err.what())==std::string("Given position or index is not valid."));
    }
    try
    {
      elem_three=BoardElement(VALUE::kCross,static_cast<POSITION>(4),3);
    }
    catch(const std::invalid_argument &err)
    {
      BOOST_CHECK(std::string(err.what())==std::string("Given position and index do not have the same value."));
    }
  }

  // test the board iterator
  BOOST_AUTO_TEST_CASE(board_iterator)
  {
    Board board;
    // create begin and end
    BoardIterator itr=board.begin(),end=board.end(),copy=board.begin();

    // test comparators
    BOOST_CHECK(itr==copy);
    BOOST_CHECK(itr!=end);

    // test begin() and operator->
    BOOST_CHECK(itr->ind==0);
    BOOST_CHECK(itr->pos==POSITION::kTopLeft);
    BOOST_CHECK(itr->val==VALUE::kEmpty);

    // test operator*
    BOOST_CHECK((*itr).ind==0);
    BOOST_CHECK((*itr).pos==POSITION::kTopLeft);
    BOOST_CHECK((*itr).val==VALUE::kEmpty);

    // test operator++ and operator=
    ++itr;
    BOOST_CHECK(itr->ind==1);
    BOOST_CHECK(itr->pos==POSITION::kTopMiddle);
    BOOST_CHECK(itr->val==VALUE::kEmpty);
    BoardIterator jtr=++itr;
    BOOST_CHECK(itr==jtr);

    // test copy constructor
    BoardIterator copyctor(itr);
    BOOST_CHECK(itr==copyctor);

    // test operator++(int) and operator=
    itr++;
    // we didn't check itr after assigning to jtr, so jump 2 not one.
    BOOST_CHECK(itr->ind==3);
    BOOST_CHECK(itr->pos==POSITION::kMiddleLeft);
    BOOST_CHECK(itr->val==VALUE::kEmpty);
    jtr=itr++;
    // check jtr is what itr was
    BOOST_CHECK(jtr!=itr);
    BOOST_CHECK(jtr->ind==3);
    BOOST_CHECK(jtr->pos==POSITION::kMiddleLeft);
    BOOST_CHECK(jtr->val==VALUE::kEmpty);
    // check itr
    BOOST_CHECK(itr->ind==4);
    BOOST_CHECK(itr->pos==POSITION::kMiddleMiddle);
    BOOST_CHECK(itr->val==VALUE::kEmpty);

    // test swapping (and implicitly the move constructor)
    swap(itr,jtr);
    // check itr is oppostie of above
    BOOST_CHECK(itr->ind==3);
    BOOST_CHECK(itr->pos==POSITION::kMiddleLeft);
    BOOST_CHECK(itr->val==VALUE::kEmpty);
    // check jtr is opposite of above
    BOOST_CHECK(jtr->ind==4);
    BOOST_CHECK(jtr->pos==POSITION::kMiddleMiddle);
    BOOST_CHECK(jtr->val==VALUE::kEmpty);

    // hard code a drawn board
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      const POSITION loc=pos;
      board.assign_position(loc,draw_one.at(static_cast<int>(loc)));
    }
    // te the iterator against the construction
    int ind=0;
    POSITION pos=POSITION::kTopLeft;
    for (const BoardElement &ktr:board)
    {
      BOOST_CHECK(ktr.ind==ind);
      BOOST_CHECK(ktr.pos==pos);
      BOOST_CHECK(ktr.val==draw_one[ind]);
      ++ind;
      ++pos;
    }
  }

  // test board isometries (<< and >> are implemented in terms of <<= and >>=)
  BOOST_AUTO_TEST_CASE(isometries)
  {
    Board board;
    // test some rotations
    board.assign_position(POSITION::kTopLeft,VALUE::kCross);
    board=board<<1;
    BOOST_CHECK(board.at(POSITION::kBottomLeft)==VALUE::kCross);
    board=board<<1;
    BOOST_CHECK(board.at(POSITION::kBottomRight)==VALUE::kCross);
    board=board<<1;
    BOOST_CHECK(board.at(POSITION::kTopRight)==VALUE::kCross);
    board.assign_position(POSITION::kMiddleLeft,VALUE::kNought);
    board=board<<3;
    BOOST_CHECK(board.at(POSITION::kBottomRight)==VALUE::kCross);
    BOOST_CHECK(board.at(POSITION::kTopMiddle)==VALUE::kNought);
    board=board<<4;
    BOOST_CHECK(board.at(POSITION::kBottomRight)==VALUE::kCross);
    BOOST_CHECK(board.at(POSITION::kTopMiddle)==VALUE::kNought);
    // test some reflections
    board=board>>1;
    BOOST_CHECK(board.at(POSITION::kBottomLeft)==VALUE::kCross);
    board=board>>2;
    BOOST_CHECK(board.at(POSITION::kBottomLeft)==VALUE::kCross);
    board.assign_position(POSITION::kMiddleRight,VALUE::kCross);
    board=board>>1;
    BOOST_CHECK(board.at(POSITION::kTopMiddle)==VALUE::kNought);
    BOOST_CHECK(board.at(POSITION::kMiddleLeft)==VALUE::kCross);
    BOOST_CHECK(board.at(POSITION::kBottomRight)==VALUE::kCross);
  }

  // test the comparison operator
  BOOST_AUTO_TEST_CASE(comparison)
  {
    Board board_one,board_two;
    board_one.assign_position(POSITION::kTopLeft,VALUE::kCross);
    board_two.assign_position(POSITION::kTopLeft,VALUE::kCross);
    BOOST_CHECK(board_one==board_two);
    
    board_one.assign_position(POSITION::kTopMiddle,VALUE::kNought);
    board_two.assign_position(POSITION::kMiddleLeft,VALUE::kNought);
    BOOST_CHECK(board_one==board_two);

    board_one.assign_position(POSITION::kBottomLeft,VALUE::kNought);
    board_two.assign_position(POSITION::kTopRight,VALUE::kNought);
    BOOST_CHECK(board_one==board_two);

    board_one.assign_position(POSITION::kBottomRight,VALUE::kNought);
    BOOST_CHECK(!(board_one==board_two));
  }

  // test swap function (and implicitly the move constructor)
  BOOST_AUTO_TEST_CASE(swapping)
  {
    Board board_one,board_two;
    /// hard code a drawn board
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      board_one.assign_position(pos,draw_one.at(static_cast<int>(pos)));
    }
    // hard code a distinctly different drawn board
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      board_two.assign_position(pos,draw_two.at(static_cast<int>(pos)));
    }
    // copy references
    Board board_one_ref=board_one,board_two_ref=board_two;
    // swap
    swap(board_one,board_two);
    // compare
    BOOST_CHECK(board_two==board_one_ref);
    BOOST_CHECK(board_one==board_two_ref);
  }

  // test board constants
  BOOST_AUTO_TEST_CASE(constants)
  {
    // constants
    std::array<POSITION,3> arr_one={POSITION::kTopLeft,POSITION::kTopMiddle,POSITION::kTopRight},
    arr_two={POSITION::kMiddleLeft,POSITION::kMiddleMiddle,POSITION::kMiddleRight},
    arr_three={POSITION::kBottomLeft,POSITION::kBottomMiddle,POSITION::kBottomRight},
    arr_four={POSITION::kTopLeft,POSITION::kMiddleLeft,POSITION::kBottomLeft},
    arr_five={POSITION::kTopMiddle,POSITION::kMiddleMiddle,POSITION::kBottomMiddle},
    arr_six={POSITION::kTopRight,POSITION::kMiddleRight,POSITION::kBottomRight},
    arr_seven={POSITION::kTopLeft,POSITION::kMiddleMiddle,POSITION::kBottomRight},
    arr_eight={POSITION::kBottomLeft,POSITION::kMiddleMiddle,POSITION::kTopRight};

    // check kWinningPositions
    BOOST_CHECK(kWinningPositions.at(0)==arr_one);
    BOOST_CHECK(kWinningPositions.at(1)==arr_two);
    BOOST_CHECK(kWinningPositions.at(2)==arr_three);
    BOOST_CHECK(kWinningPositions.at(3)==arr_four);
    BOOST_CHECK(kWinningPositions.at(4)==arr_five);
    BOOST_CHECK(kWinningPositions.at(5)==arr_six);
    BOOST_CHECK(kWinningPositions.at(6)==arr_seven);
    BOOST_CHECK(kWinningPositions.at(7)==arr_eight);
  }
  
BOOST_AUTO_TEST_SUITE_END()
} // namespace BoardTesting
