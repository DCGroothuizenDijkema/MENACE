
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// test-game.hpp                                                                                                                         //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Test file for the Game class of MEANCE


#include <game.hpp>
 
namespace GameTesting
{
using namespace menace;

const std::array<POSITION,9> draw_one={
  POSITION::kBottomLeft,POSITION::kMiddleMiddle,POSITION::kTopMiddle,POSITION::kMiddleRight,POSITION::kMiddleLeft,POSITION::kTopLeft
  ,POSITION::kBottomRight,POSITION::kBottomMiddle,POSITION::kTopRight
};

BOOST_AUTO_TEST_SUITE(test_game)

  // test initialising game objects
  BOOST_AUTO_TEST_CASE(initialisation)
  {
    Board board;
    Player player_one(VALUE::kCross,board),player_two(VALUE::kNought,board),player_three(VALUE::kCross,board)
      ,player_four(VALUE::kNought,board);

    // check default is non throwing
    BOOST_CHECK_NO_THROW(Game(board,&player_one,&player_two,GAME::kPvP));
    // check cross-cross is throwing 
    BOOST_CHECK_THROW(Game(board,&player_one,&player_three,GAME::kPvP),matching_player_values);
    // check error is constructed correctly
    try
    {
      Game game=Game(board,&player_one,&player_three,GAME::kPvP);
    }
    catch (const matching_player_values &err)
    {
      BOOST_CHECK(err.val==player_one.value);
    }
    // check nought-nought is throwing
    BOOST_CHECK_THROW(Game(board,&player_two,&player_four,GAME::kPvP),matching_player_values);
    // check error is constructed correctly
    try
    {
      Game game=Game(board,&player_two,&player_four,GAME::kPvP);
    }
    catch (const matching_player_values &err)
    {
      BOOST_CHECK(err.val==player_two.value);
    }
  }

  // test throws from invalid player and game type combinations
  // this is pretty brute-forcey, and because the programme doesn't actually use the error, it's configuration isn't tested, sorry
  BOOST_AUTO_TEST_CASE(invalid_initialisation)
  {
    Board board;
    Player player_one(VALUE::kCross,board),player_two(VALUE::kNought,board);
    MENACE menace_one(VALUE::kCross,board),menace_two(VALUE::kNought,board);
    Random random_one(VALUE::kCross,board),random_two(VALUE::kNought,board);

    // check person v person
    BOOST_CHECK_NO_THROW(Game(board,&player_one,&player_two,GAME::kPvP));
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kPvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kPvP),invalid_game_player_combination);

    // check person v menace
    BOOST_CHECK_NO_THROW(Game(board,&player_one,&menace_two,GAME::kPvM));
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kPvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kPvM),invalid_game_player_combination);

    // check person v random
    BOOST_CHECK_NO_THROW(Game(board,&player_one,&random_two,GAME::kPvR));
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kPvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kPvR),invalid_game_player_combination);

    // check menace v person
    BOOST_CHECK_NO_THROW(Game(board,&menace_one,&player_two,GAME::kMvP));
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kMvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kMvP),invalid_game_player_combination);

    // check menace v menace
    BOOST_CHECK_NO_THROW(Game(board,&menace_one,&menace_two,GAME::kMvM));
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kMvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kMvM),invalid_game_player_combination);

    // check menace v random
    BOOST_CHECK_NO_THROW(Game(board,&menace_one,&random_two,GAME::kMvR));
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kMvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kMvR),invalid_game_player_combination);

    // check random v person
    BOOST_CHECK_NO_THROW(Game(board,&random_one,&player_two,GAME::kRvP));
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kRvP),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kRvP),invalid_game_player_combination);

    // check random v menace
    BOOST_CHECK_NO_THROW(Game(board,&random_one,&menace_two,GAME::kRvM));
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kRvM),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&random_two,GAME::kRvM),invalid_game_player_combination);

    // check random v random
    BOOST_CHECK_NO_THROW(Game(board,&random_one,&random_two,GAME::kRvR));
    BOOST_CHECK_THROW(Game(board,&player_one,&player_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&menace_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&player_one,&random_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&player_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&menace_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&menace_one,&random_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&player_two,GAME::kRvR),invalid_game_player_combination);
    BOOST_CHECK_THROW(Game(board,&random_one,&menace_two,GAME::kRvR),invalid_game_player_combination);
  }

  // test game play and getters
  BOOST_AUTO_TEST_CASE(game_play)
  {
    Board board;
    Player player_one(VALUE::kCross,board),player_two(VALUE::kNought,board);
    Game game(board,&player_one,&player_two,GAME::kPvP);
    // check counter
    BOOST_CHECK(game.get_plays()==0);
    for (int itr=0;itr<9;++itr)
    {
      // check no throw
      BOOST_CHECK_NO_THROW(game.play(draw_one[itr]));
      // check counter is incremented
      BOOST_CHECK(game.get_plays()==itr+1);
    }
  }

BOOST_AUTO_TEST_SUITE_END()
}
