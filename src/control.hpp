
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// control.hpp                                                                                                                           //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Control class of MENACE


#pragma once

#ifndef CONTROL_H__
#define CONTROL_H__

#include <memory>

#include <board.hpp>
#include <game.hpp>
#include <player.hpp>
#include <ui.hpp>

namespace menace
{
// Exceptions
class unknown_play_option : public std::exception
{
public:
  unknown_play_option(const std::vector<std::string> &option) : option(option) {};

  const std::vector<std::string> option;
};

// Classes
class Control
{
public:
  Control();
  ~Control();

  void run(void);

private:
  UI ui;
  Board board;
  // humans, menaces, and randoms exist for the enite programme runtime
  // automaton_one and automaton_two change to meance, or random they point for each game
  Player human_one,human_two;
  MENACE menace_one,menace_two;
  Random random_one,random_two;

  Automaton *automaton_one,*automaton_two;

  void play(void);

  std::unique_ptr<Game> parse_game(const std::vector<std::string> &game_type);
};
}

#endif // CONTROL_H__
