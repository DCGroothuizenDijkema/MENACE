
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// menace.hpp                                                                                                                            //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Project header file of the C++ MENACE implementation
// enums, functions, and constants used across multiple files are herein defined


#pragma once

#ifndef MENACE_H__
#define MENACE_H__

#include <algorithm>
#include <array>
#include <exception>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <vector>

namespace menace
{
// Enums
enum class COLOUR
{
  kRed=0
  , kGreen
  , kYellow
  , kBlue
  , kPurple
  , kCyan
};

enum class GAME
{
  kPvP=0
  , kPvM
  , kMvP
  , kMvM
  , kRvR
  , kMvR
  , kRvM
  , kRvP
  , kPvR
};

enum class ITERATION
{
  kZero=0
  , kOne
  , kTwo
  , kThree
  , kFour
  , kFive
  , kSix
  , kSeven
  , kEight
  , kNine
};

enum class PLAYER
{
  kPlayer=0
  , kMENACE
  , kRandom
};

enum class POSITION
{
  kTopLeft=0
  , kTopMiddle
  , kTopRight
  , kMiddleLeft
  , kMiddleMiddle
  , kMiddleRight
  , kBottomLeft
  , kBottomMiddle
  , kBottomRight
};

enum class RESULT
{
  kNotWon=0
  , kCross
  , kNought
  , kDraw
};

enum class REQUEST
{
  kAdvise=0
  , kMove
};

enum class VALUE
{
  kEmpty=0
  , kCross
  , kNought
};

// Enum operator overloads
// equality to an integer
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline bool operator==(const T i, const int n)
{
  return static_cast<int>(i)==n;
}
// inequality to an enum
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline bool operator!=(const T lhs, const T rhs)
{
  return static_cast<int>(lhs)!=static_cast<int>(rhs);
}
// inequality to an integer
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline bool operator!=(const T lhs, const int rhs)
{
  return static_cast<int>(lhs)!=rhs;
}
// less than or equal to an enum
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline bool operator<=(const T lhs, const T rhs)
{
  return static_cast<int>(lhs)<=static_cast<int>(rhs);
}
// less than an int
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline bool operator<(const T lhs, const int rhs)
{
  return static_cast<int>(lhs)<rhs;
}
// greater than an in
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline bool operator>(const T lhs, const int rhs)
{
  return static_cast<int>(lhs)>rhs;
}
// modulo an int
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T operator%(T i, int n)
{
  return static_cast<T>(static_cast<int>(i)%n);
}
// prefix ++
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T &operator++(T &i)
{
  i=static_cast<T>(static_cast<int>(i)+1);
  return i;
}
// postfix ++
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T operator++(T &i, int)
{
  T res=i;
  ++i;
  return res;
}
// plus an int
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T operator+(const T t, const int n)
{
  return static_cast<T>(static_cast<int>(t)+n);
}
// minus an int
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T operator-(const T t, const int n)
{
  return static_cast<T>(static_cast<int>(t)-n);
}
// multiply an int
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T operator*(const T t, const int n)
{
  return static_cast<T>(static_cast<int>(t)*n);
}
// divided by an int
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T operator/(const T t, const int n)
{
  return static_cast<T>(static_cast<int>(t)/n);
}
// plus equals an int
template <class T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
constexpr inline T &operator+=(T &t, const int n)
{
  t=static_cast<T>(static_cast<int>(t)+n);
  return t;
}

// Inlines
// Vector addition
template <typename T>
inline std::vector<T> operator+(const std::vector<T> &lhs, const std::vector<T> &rhs)
{
  std::vector<T> out=lhs;
  std::copy(std::cbegin(rhs),std::cend(rhs),std::back_inserter(out));
  return out;
}
// finding an element
template <typename Container, typename T>
inline int find(const Container &container, const T val)
{
  typename Container::const_iterator begin=std::cbegin(container),end=std::cend(container),loc=std::find(begin,end,val);
  // it can't be found
  if (loc==end) { return -1; }
  // return position
  return static_cast<int>(std::distance(begin,loc));
}

// Constants

// kUnhelpableCommands through to kUserRequests should be defined in ui.hpp, but they are here for testing
// weird compiler errors result otherwise to do with the WINDOWS namespace

const std::array<std::string,3> kUnhelpableCommands={{
  "help"
  , "cls"
  , "exit"
}};

const std::array<std::string,9> kPositions={{
  "topleft"
  , "topmiddle"
  , "topright"
  , "middleleft"
  , "middlemiddle"
  , "middleright"
  , "bottomleft"
  , "bottommiddle"
  , "bottomright"
}};

const std::array<std::string,3> kRowNames={{
  "top"
  , "middle"
  , "bottom"
}};

const std::array<std::string,4> kGameExitCommands={{

  "quit"
  , "q"
  , "break"
  , "exit"
}};

const std::map<COLOUR,std::string> kColours={
  {COLOUR::kRed,"\033[1;31m"}
  , {COLOUR::kGreen,"\033[1;32m"}
  , {COLOUR::kYellow,"\033[1;33m"}
  , {COLOUR::kBlue,"\033[1;34m"}
  , {COLOUR::kPurple,"\033[1;35m"}
  , {COLOUR::kCyan,"\033[1;36m"}
};

const std::string kColourEnd="\033[0m";

const std::map<std::string,REQUEST> kUserRequests={
  {"advise",REQUEST::kAdvise}
  , {"move",REQUEST::kMove}
};

const std::map<VALUE,std::string> kValueStrings={{

  {VALUE::kEmpty," "}
  , {VALUE::kCross,"X"}
  , {VALUE::kNought,"O"}
}};

const std::map<VALUE,COLOUR> kValueColours={{

  {VALUE::kEmpty,COLOUR::kRed} // just a filler, never gets printed
  , {VALUE::kCross,COLOUR::kPurple}
  , {VALUE::kNought,COLOUR::kBlue}
}};

const std::map<POSITION,std::string> kPositionStrings={{

  {POSITION::kTopLeft,"top left"}
  , {POSITION::kTopMiddle,"top middle"}
  , {POSITION::kTopRight,"top right"}
  , {POSITION::kMiddleLeft,"middle left"}
  , {POSITION::kMiddleMiddle,"middle middle"}
  , {POSITION::kMiddleRight,"middle right"}
  , {POSITION::kBottomLeft,"bottom left"}
  , {POSITION::kBottomMiddle,"bottom middle"}
  , {POSITION::kBottomRight,"bottom right"}
}};

const std::array<GAME,6> kHumanIncludedGames={{

  GAME::kPvP
  , GAME::kPvR
  , GAME::kPvM
  , GAME::kMvP
  , GAME::kRvP
}};

const std::array<GAME,6> kHumanFirstGames={{

  GAME::kPvP
  , GAME::kPvR
  , GAME::kPvM
}};

const std::array<GAME,6> kHumanSecondGames={{

  GAME::kPvP
  , GAME::kMvP
  , GAME::kRvP
}};

const std::array<GAME,6> kAutomatonFirstGames={{

  GAME::kMvM
  , GAME::kMvP
  , GAME::kMvR
  , GAME::kRvR
  , GAME::kRvP
  , GAME::kRvM
}};

const std::array<GAME,6> kAutomatonSecondGames={{

  GAME::kMvM
  , GAME::kPvM
  , GAME::kRvM
  , GAME::kRvR
  , GAME::kPvR
  , GAME::kMvR
}};

const std::array<GAME,3> kMENACEFirstGames={{

  GAME::kMvM
  , GAME::kMvP
  , GAME::kMvR
}};

const std::array<GAME,3> kMENACESecondGames={{

  GAME::kMvM
  , GAME::kPvM
  , GAME::kRvM
}};

const std::array<GAME,3> kRandomFirstGames={{

  GAME::kRvR
  , GAME::kRvP
  , GAME::kRvM
}};

const std::array<GAME,3> kRandomSecondGames={{

  GAME::kRvR
  , GAME::kPvR
  , GAME::kMvR
}};
} // namespace menace

#endif // MENACE_H__
