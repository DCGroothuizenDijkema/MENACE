
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// test-player.hpp                                                                                                                       //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Test file for the Player classes of MENACE


#include <player.hpp>

namespace PlayerTesting
{
using namespace menace; 

const std::array<VALUE,9> draw_one={
  VALUE::kCross,VALUE::kNought,VALUE::kCross,VALUE::kCross,VALUE::kNought,VALUE::kCross,VALUE::kNought,VALUE::kCross,VALUE::kNought
};

BOOST_AUTO_TEST_SUITE(test_player)

  // test initialising player objects
  BOOST_AUTO_TEST_CASE(initialisation)
  {
    Board board;
    // check no throws with VALUE::kCross and VALUE::kNought
    BOOST_CHECK_NO_THROW(Player(VALUE::kCross,board));
    BOOST_CHECK_NO_THROW(Player(VALUE::kNought,board));
    // check a player cannot be VALUE::kEmpty
    BOOST_CHECK_THROW(Player(VALUE::kEmpty,board),assigning_empty_player);
  }

  // test the identity function
  BOOST_AUTO_TEST_CASE(identity)
  {
    Board board;
    Player player=Player(VALUE::kCross,board);
    // check it has the right value
    BOOST_CHECK(player.ident()==PLAYER::kPlayer);
  }

  // test a player can play
  BOOST_AUTO_TEST_CASE(playing)
  {
    Board board;
    Player player=Player(VALUE::kCross,board);
    // check no throws for playing
    BOOST_CHECK_NO_THROW(player.make_move(POSITION::kTopLeft));
    BOOST_CHECK_NO_THROW(player.make_move(POSITION::kBottomRight));
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_menace)

  // test the identity function
  BOOST_AUTO_TEST_CASE(identity)
  {
    Board board;
    MENACE player=MENACE(VALUE::kCross,board);
    // check it has the right value
    BOOST_CHECK(player.ident()==PLAYER::kMENACE);
  }

  // test the minimax algorithm
  // this will be difficult, and i'm not sure if this counts as rigorous testing, but MENACE should always draw against itself,
  // and never lose to a random, so we'll play a bunch of games and checks this happens.
  BOOST_AUTO_TEST_CASE(playing)
  {
    for (int itr=0;itr<10/*seems reasonable...*/;++itr)
    {
      Board board;
      MENACE player_one=MENACE(VALUE::kCross,board),player_two=MENACE(VALUE::kNought,board);
      Game game=Game(board,&player_one,&player_two,GAME::kMvM);
      // play a game until it's over
      while (board.check_board()==RESULT::kNotWon)
      {
        POSITION pos;
        if (game.get_plays()%2==0)
        {
          pos=player_one.get_move();
        }
        else if (game.get_plays()%2==1)
        {
          pos=player_two.get_move();
        }
        BOOST_CHECK_NO_THROW(game.play(pos));
      }
      // check it's a draw
      BOOST_CHECK(board.check_board()==RESULT::kDraw);
    }

    for (int itr=0;itr<10/*seems reasonable...*/;++itr)
    {
      Board board;
      Random player_one=Random(VALUE::kCross,board); // let's even give Random the upper hand
      MENACE player_two=MENACE(VALUE::kNought,board);
      Game game=Game(board,&player_one,&player_two,GAME::kRvM);
      // play a game until it's over
      while (board.check_board()==RESULT::kNotWon)
      {
        POSITION pos;
        if (game.get_plays()%2==0)
        {
          pos=player_one.get_move();
        }
        else if (game.get_plays()%2==1)
        {
          pos=player_two.get_move();
        }
        BOOST_CHECK_NO_THROW(game.play(pos));
      }
      // check Random hasn't won
      BOOST_CHECK(board.check_board()!=RESULT::kCross);
    }
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_random)

  // test the identity function
  BOOST_AUTO_TEST_CASE(identity)
  {
    Board board;
    Random player=Random(VALUE::kCross,board);
    // check it has the right value
    BOOST_CHECK(player.ident()==PLAYER::kRandom);
  }

  // test random's position choosing
  BOOST_AUTO_TEST_CASE(playing)
  {
    // randomly selecting a position on an empty board gives us any position
    for (int itr=0;itr<1000/*seems reasonable...*/;++itr)
    {
      Board board;
      Random player=Random(VALUE::kCross,board);
      POSITION pos=player.get_move();
      // check the position is in range
      BOOST_CHECK(pos<9);
      BOOST_CHECK(pos>-1);
    }

    // randomly selecting a position on an all but one full board always gives us the only position available
    for (int itr=0;itr<1000/*seems reasonable...*/;++itr)
    {
      Board board;
      Random player=Random(VALUE::kCross,board);
      // hard code a drawn board
      for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomMiddle;++pos)
      {
        const POSITION loc=pos;
        board.assign_position(loc,draw_one.at(static_cast<int>(loc)));
      }
      POSITION pos=player.get_move();
      // check the right position is chosen
      BOOST_CHECK(pos==POSITION::kBottomRight);
    }
  }

BOOST_AUTO_TEST_SUITE_END()
} // namespace PlayerTesting
