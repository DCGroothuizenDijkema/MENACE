
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// game.cpp                                                                                                                              //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Implementation of the Game class of MENACE


#include <game.hpp>
#include <iostream>

namespace menace
{
Game::Game(Board &board, Player *player_one, Player *player_two, const GAME type) : board(board), players({player_one,player_two})
  , type(type)
{
  //
  // Game class constructor
  // this->board and this->type are set to corresponding inputs, this->players is constructed from input players
  //
  // parameters
  // ----------
  // board : Board &
  //  - the Board the game will be played on
  // player_one,player_two : Player *
  //  - the two players to play the game
  // type : const GAME
  //  - the type of game, encoding if the players are people, random, or MENACEs
  //
  // throws
  // ------
  // matching_player_values
  //  - if the two players have the same VALUE
  // invalid_game_player_combination
  //  - if the types of the players do not match the description in the game
  //
  
  if (player_one->value==player_two->value) { throw matching_player_values(player_one->value); }
  // check the players match the types
  if (
    find(kMENACEFirstGames,type)!=-1&&player_one->ident()!=PLAYER::kMENACE ||
    find(kMENACESecondGames,type)!=-1&&player_two->ident()!=PLAYER::kMENACE ||
    find(kRandomFirstGames,type)!=-1&&player_one->ident()!=PLAYER::kRandom ||
    find(kRandomSecondGames,type)!=-1&&player_two->ident()!=PLAYER::kRandom ||
    find(kHumanFirstGames,type)!=-1&&player_one->ident()!=PLAYER::kPlayer ||
    find(kHumanSecondGames,type)!=-1&&player_two->ident()!=PLAYER::kPlayer
  )
  { throw invalid_game_player_combination(player_one->value,player_two->value,type); }
}

Game::~Game()
{
  //
  // Game class destructor
  //
}

void Game::play(const POSITION pos)
{
  //
  // Make a move on the board
  // Determines whose move it is based on the number of plays, and makes that player move on the given position
  //
  // parameters
  // ----------
  // pos : const POSITION
  //  - the POSITION to move on
  //

  this->players[static_cast<int>(this->get_plays()%2)]->make_move(pos);
}

[[nodiscard]] ITERATION Game::get_plays(void) const
{
  //
  // Game::board::plays getter
  //
  // returns
  // -------
  // ITERATION
  //  - the number of moves made on the board
  //

  return this->board.get_plays();
}
} // namespace menace
