
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// player.hpp                                                                                                                            //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Player classes of MENACE


#pragma once

#ifndef PLAYER_H__
#define PLAYER_H__

#include <functional>
#include <random>

#include <board.hpp>

namespace menace
{
// Exceptions
class assigning_empty_player : public std::exception {};

// Classes
// Player is directly used for humans, inherited for the computer
class Player
{
public:
  Player(const VALUE value, Board &board);
  void make_move(POSITION pos) const;
  
  const VALUE value;

  [[nodiscard]] virtual PLAYER ident(void) const;

protected:
  Board &board;
};

// virtual base for Random and MENACE
class Automaton : public Player
{
public:
  Automaton(const VALUE value, Board &board);
  virtual POSITION get_move(void)=0;

  [[nodiscard]] virtual PLAYER ident(void) const=0;

protected:
  bool valid_move(const Board &state, POSITION pos) const;
  bool valid_move(const POSITION pos) const;

  std::vector<POSITION> determine_possible_moves(void) const;
  std::vector<POSITION> determine_possible_moves(const Board &state) const;
};

class MENACE : public Automaton
{
public:
  MENACE(const VALUE value, Board &board);

  POSITION get_move(void);
  void clear_tansposition_table(void);

  [[nodiscard]] PLAYER ident(void) const;

private:
  int minimax(const Board &state, int alpha, int beta, const bool maximising_player, const VALUE maximiser_value
    , const VALUE minimiser_value);

  
  // transposition table
  std::vector<Board> evaluated_boards;
  std::vector<int> evaluations;
};

class Random : public Automaton
{
public:
  Random(const VALUE value, Board &board);
  POSITION get_move(void);

  [[nodiscard]] PLAYER ident(void) const;
};
} // namespace menace

#endif // PLAYER_H__
