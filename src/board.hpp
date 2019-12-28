
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// board.hpp                                                                                                                             //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Board and related classes of MENACE


#pragma once

#ifndef BOARD_H__
#define BOARD_H__

#include <menace.hpp>

namespace menace
{
// Exceptions
class non_empty_position : public std::exception
{
public:
  non_empty_position(const POSITION pos, const VALUE val) : pos(pos), val(val) {};

  const POSITION pos;
  const VALUE val;
};

class game_finished : public std::exception
{
public:
  game_finished(const RESULT result) : result(result) {};

  const RESULT result;
};

class assigning_empty_value : public std::exception {};

// Constants
const std::array<std::array<POSITION,3>,8> kWinningPositions={{
  // rows
  {{POSITION::kTopLeft,POSITION::kTopMiddle,POSITION::kTopRight}}
  , {{POSITION::kMiddleLeft,POSITION::kMiddleMiddle,POSITION::kMiddleRight}}
  , {{POSITION::kBottomLeft,POSITION::kBottomMiddle,POSITION::kBottomRight}}
  // columns
  , {{POSITION::kTopLeft,POSITION::kMiddleLeft,POSITION::kBottomLeft}}
  , {{POSITION::kTopMiddle,POSITION::kMiddleMiddle,POSITION::kBottomMiddle}}
  , {{POSITION::kTopRight,POSITION::kMiddleRight,POSITION::kBottomRight}}
  // diagonals
  , {{POSITION::kTopLeft,POSITION::kMiddleMiddle,POSITION::kBottomRight}}
  , {{POSITION::kBottomLeft,POSITION::kMiddleMiddle,POSITION::kTopRight}}
}};

// Classes
class BoardElement
{
public:
  VALUE val;
  POSITION pos;
  size_t ind;

  BoardElement();
  BoardElement(const VALUE val, const POSITION pos, const size_t ind);
  ~BoardElement();

private:
  bool check_range(const size_t idx);
};

class BoardIterator
{
  using const_board_iterator=std::array<VALUE,9>::const_iterator;

  const_board_iterator board_itr;
  BoardElement elem;

public:
  using pointer=const BoardElement * const;
  using reference=const BoardElement &;

  using iterator_category=std::array<VALUE,9>::const_iterator::iterator_category;
  using difference_type=std::array<VALUE,9>::const_iterator::difference_type;
  using value_type=std::array<VALUE,9>::const_iterator::value_type;

  BoardIterator::BoardIterator();
  BoardIterator(const const_board_iterator begin, const const_board_iterator board_itr);
  BoardIterator(const BoardIterator &other);
  BoardIterator(BoardIterator &&other);
  ~BoardIterator();

  // assignment
  BoardIterator &operator=(BoardIterator other);
  // dereference
  reference operator*() const;
  pointer operator->() const;
  // increment
  BoardIterator &operator++();
  BoardIterator operator++(int);
  // comparison
  friend inline bool operator==(const BoardIterator & lhs, const BoardIterator &rhs);
  friend inline bool operator!=(const BoardIterator & lhs, const BoardIterator &rhs);
  // swap
  friend void swap(BoardIterator& first, BoardIterator& second) noexcept;
};

class Board
{
public:
  Board();
  Board(const Board &other);
  Board(Board &&other);
  ~Board();

  void assign_position(POSITION pos, const VALUE value);
  void clear_board(void);
  RESULT check_board(void) const;

  // iterators
  [[nodiscard]] BoardIterator begin(void) const;
  [[nodiscard]] BoardIterator end(void) const;

  // getters
  [[nodiscard]] ITERATION get_plays(void) const;
  [[nodiscard]] const VALUE &at(const POSITION &pos);
  [[nodiscard]] const VALUE &at(const POSITION &pos) const;
  [[nodiscard]] const VALUE &operator[](const POSITION &pos);
  [[nodiscard]] const VALUE &operator[](const POSITION &pos) const;

  // assignment
  Board &operator=(Board other);
  Board &operator>>=(const int n);
  Board &operator<<=(int n);
  // comparison
  friend inline bool operator==(const Board &lhs, const Board &rhs);
  // isometries
  friend inline Board operator>>(Board lhs, const int n);
  friend inline Board operator<<(Board lhs, const int n);
  // io
  friend std::ostream &operator<<(std::ostream &os, const Board &board); // defined in ui.cpp to make use of coloured text
  // swap
  friend void swap(Board& first, Board& second) noexcept;

private:
  ITERATION plays;
  std::array<VALUE,9> board;
  int cross_count,nought_count;

  bool check_equal(const std::array<POSITION, 3> &pos) const;
  bool check_range(const POSITION pos) const;

  // getters
  [[nodiscard]] VALUE &at(POSITION &pos);
  [[nodiscard]] VALUE &operator[](POSITION &pos);
};

// Inline Overloads
inline bool operator==(const BoardIterator & lhs, const BoardIterator &rhs)
{
  //
  // BoardIterator operator==
  // Compare two instances of BoardIterator
  //
  // parameters
  // ----------
  // lhs,rhs : const BoardIterator &
  //  - the port and starboard sides of the equality
  //
  // returns
  // -------
  // bool
  //  - true if their iterator is the same, false otherwise
  //

  return lhs.board_itr==rhs.board_itr;
}

inline bool operator!=(const BoardIterator &lhs, const BoardIterator &rhs)
{
  //
  // BoardIterator operator!=
  // Compare two instances of BoardIterator
  //
  // parameters
  // ----------
  // lhs,rhs : const BoardIterator &
  //  - the port and starboard sides of the inequality
  //
  // returns
  // -------
  // bool
  //  - false if their iterator is the same, true otherwise
  //

  return !(lhs==rhs);
}

inline bool operator==(const Board &lhs, const Board &rhs)
{
  //
  // Board operator==
  // Compare two instances of Board where isometries do not matter
  //
  // parameters
  // ----------
  // lhs,rhs : const Board &
  //  - the port and starboard sides of the equality
  //
  // returns
  // -------
  // bool
  //  - true if any isometries of lhs::board and rhs::board match, false otherwise
  //

  // if they don't have the same number of each, they can never be equal
  if (!(lhs.cross_count==rhs.cross_count&&lhs.nought_count==rhs.nought_count)) { return false; }
  
  // check all permutations
  for (int itr=0;itr<2;++itr)
  {
    Board tmp=lhs;
    tmp>>=itr; // flip
    for (int jtr=0;jtr<4;jtr++)
    {
      tmp<<=1; // rotate
      // only one isometry has to match
      if (tmp.board==rhs.board) { return true; }
    }
  }
  return false;
}

inline Board operator<<(Board lhs, const int n)
{
  //
  // Board operator<<
  // rotate Board::board n quater turns counterclockwise
  //
  // parameters
  // ----------
  // lhs : Board
  //  - the Board to rotate
  // n : const int 
  //  - number of turns to make
  //
  // returns
  // -------
  // Board
  //  - rotated board
  //

  lhs<<=n;
  return lhs;
}

inline Board operator>>(Board lhs, const int n)
{
  //
  // Board operator>>
  // Reflect Board::board along its vertical axis.
  //
  // parameters
  // ----------
  // lhs : Board
  //  - the Board to flip
  // n : const int 
  //  - number of reflections to make
  //    if n%2==0, there is no reflection; if n%2==1, there is one reflection
  //
  // returns
  // -------
  // Board
  //  - reflected board
  //

  lhs>>=n;
  return lhs;
}
} // namespace menace

#endif // BOARD_H__
