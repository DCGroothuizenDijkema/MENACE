
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// game.hpp                                                                                                                              //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Game class of MENACE


#pragma once

#ifndef GAME_H__
#define GAME_H__

#include <board.hpp>
#include <menace.hpp>
#include <player.hpp>

namespace menace
{
// Exceptions
class matching_player_values : public std::exception
{
public:
  matching_player_values(const VALUE val) : val(val) {};

  const VALUE val;
};

class invalid_game_player_combination : public std::exception
{
public:
  invalid_game_player_combination(const VALUE player_one_val, const VALUE player_two_val, const GAME type) : player_one_val(player_one_val)
  , player_two_val(player_two_val), type(type) {};

  const VALUE player_one_val,player_two_val;
  const GAME type;
};

// Classes
class Game
{
public:
  Game(Board &board, Player *player_one, Player *player_two, const GAME type);
  ~Game();

  void play(const POSITION pos);

  [[nodiscard]] ITERATION get_plays(void) const;

  const GAME type;
  const std::array<const Player * const,2> players;
private:
  Board &board;
};
} // namespace menace

#endif // GAME_H__
