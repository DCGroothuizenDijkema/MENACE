
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// ui.hpp                                                                                                                                //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// UI class of MENACE


#pragma once
#ifndef UI_H__
#define UI_H__

#include <cctype>

#include <iomanip>
#include <iostream>

namespace WINDOWS
{
#include <windows.h>
} // namespace WINDOWS

#ifdef STD_OUTPUT_HANDLE
#undef STD_OUTPUT_HANDLE
#define STD_OUTPUT_HANDLE ((WINDOWS::DWORD)-11)
#endif // STD_OUTPUT_HANDLE

#include <boost/algorithm/string.hpp>

#include <board.hpp>

namespace menace
{
// Enums
enum class ERR
{
  kError=0
  , kUnkownCommand
  , kInvalidPosition
  , kUnknownPosition
  , kInvalidArguments
  , kInvalidArgumentNumber
  , kInvalidHelpArgument
};

enum class CLEAR
{
  kPlayHelp=0
};

enum class HELP
{
  kPlay=0
  , kReset
};

// Exceptions
class early_exit : public std::exception {};

class unknown_position : public std::exception
{
public:
  unknown_position(const std::string &pos) : pos(pos) {};

  const std::string pos;
};

class invalid_print_args : public std::exception
{
public:
  invalid_print_args(const int expected, const size_t received) : expected(expected), received(received) {};

  const int expected;
  const size_t received;
};

class unattributed_colour : public std::exception
{
public:
  unattributed_colour(const COLOUR colour) : colour(colour) {};

  const COLOUR colour;
};

class user_play_request : public std::exception
{
public:
  user_play_request(const REQUEST request) : request(request) {};

  const REQUEST request;
};

// Classes
class UI
{
public:
  UI(const Board *display_board);
  ~UI();

  const Board *display_board;

  // output
  void print_error(const ERR error, const std::vector<std::string> &args={});
  void print_error(const ERR error, const WINDOWS::COORD &origin, const std::vector<std::string> &args={});
  void print_help(void);
  void print_help(const HELP help);
  void print_exit(void);
  void print_board(void);
  void print_board(const WINDOWS::COORD &origin);
  void print_result(const RESULT result, const GAME game);
  void print_menace(void);
  void print_play_help(void);

  // input
  std::vector<std::string> get_command(size_t &argc);
  POSITION get_move(const ITERATION itr);
  
  // terminal manipulation
  void move(const short x, const short y, const WINDOWS::COORD &origin);
  void clear(const short x, const short y, const int n, const WINDOWS::COORD &origin);
  void clear(const int n);
  void clear(const CLEAR type, const WINDOWS::COORD &origin);
  void clear(void);
  [[nodiscard]] WINDOWS::COORD origin(void);

  void set_advisial(const POSITION pos);
  void reset_advisial(void);

  friend std::ostream &operator<<(std::ostream &os, const Board &board);

private:
  void intro_screen(void);
  void topic_screen(void);

  static std::string colour_text(const std::string &s, const COLOUR colour);

  std::string blank(const int n);
  static const WINDOWS::HANDLE std_handle;

  static std::array<bool,9> advisial;
};
} // namespace menace

#endif // UI_H__
