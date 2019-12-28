
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// board.cpp                                                                                                                             //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Implementation of the Board and related iterator classes of MENACE


#include <board.hpp>

namespace menace
{
BoardElement::BoardElement() : val(VALUE::kEmpty), pos(POSITION::kTopLeft), ind(0)
{
  //
  // BoardElement class default constructor
  // this->val is set to VALUE::kEmpty (0), this->pos to POSITION::kTopLeft (0), and this->ind to 0
  //
}

BoardElement::BoardElement(const VALUE val, const POSITION pos, const size_t ind) : val(val), pos(pos), ind(ind)
{
  //
  // BoardElement class constructor
  // this->val, this->pos, and this->ind are set to corresponding inputs
  //
  // parameters
  // ----------
  // val : const VALUE
  //  - the value of the Board element
  // pos : const POSITION 
  //  - the position of the Board element
  // ind : const size_t
  //  - the indexable representation of the position
  //
  // throws
  // ------
  // std::invalid_argument
  //  - if pos and ind are not identical or exceed range limits
  //

  if (static_cast<int>(pos)!=static_cast<int>(ind)) { throw std::invalid_argument("Given position and index do not have the same value."); }
  if (!this->check_range(ind)) { throw std::invalid_argument("Given position or index is not valid."); }
}

BoardElement::~BoardElement()
{
  //
  // BoardElement class destructor
  //
}

bool BoardElement::check_range(const size_t idx)
{
  //
  // Check if a given index is in range for a Board iterator
  // The index is out of range if its integral value is strictly less than 0 or strictly greater than 9
  //
  // parameters
  // ----------
  // ind : const size_t
  //  - the index to check against
  //
  // returns
  // -------
  // bool
  //  - false if the index is out of range, true otherwise
  //

  // greater than 9 for end iterator, but 8 is the end of the board
  if (idx<0||idx>9) { return false; }
  return true;
}

BoardIterator::BoardIterator() : board_itr(), elem()
{
  //
  // BoardIterator class default constructor
  // this->board_itr and this->elem are set to defaults
  //
}

BoardIterator::BoardIterator(const const_board_iterator begin, const const_board_iterator board_itr) : board_itr(board_itr)
{
  //
  // BoardIterator class constructor
  // this->board_itr is set to corresponding input. this->elem is constructed from the inputs
  //
  // parameters
  // ----------
  // begin,board_itr : const BoardIterator::const_board_iterator
  //  - the beginning of the board, for calculating the distance, and a board iterator to be incremented
  //

  difference_type distance=std::distance(begin,board_itr);
  elem=BoardElement(*board_itr,static_cast<POSITION>(distance),static_cast<size_t>(distance));
}

BoardIterator::BoardIterator(const BoardIterator &other) : board_itr(other.board_itr), elem(other.elem)
{
  //
  // BoardIterator class copy constructor
  // this->begin is set to other.begin, this->board_itr is set to other.board_itr, and this-> is set to other.elem
  //
  // parameters
  // ----------
  // other : const BoardIterator &
  //  - the BoardIterator class object from which to produce a copy
  //
}

BoardIterator::BoardIterator(BoardIterator &&other)
{
  //
  // BoardIterator class move constructor
  // other is moved into *this
  //
  // parameters
  // ----------
  // other : BoardIterator &&
  //  - the BoardIterator class object to move from
  //

  swap(*this,other);
}

BoardIterator::~BoardIterator()
{
  //
  // BoardIterator class destructor
  //
}

BoardIterator &BoardIterator::operator=(BoardIterator other)
{
  //
  // BoardIterator operator=
  // Assign a BoardIterator object to the BoardIterator in *this using copy-and-swap
  //
  // parameters
  // ----------
  // other : BoardIterator
  //  - the BoardIterator object to assign from
  //
  // returns
  // -------
  // BoardIterator &
  //  - the BoardIterator in other now in *this
  //

  swap(*this,other);
  return *this;
}

BoardIterator::reference BoardIterator::operator*() const
{
  //
  // BoardIterator operator*
  // Indirects to a const reference to this->elem
  //
  // returns
  // -------
  // BoardIterator::reference
  //  - a reference to this->elem
  //

  return this->elem;
}

BoardIterator::pointer BoardIterator::operator->() const
{
  //
  // BoardIterator operator->
  // Indirects to a pointer to this->elem
  //
  // returns
  // -------
  // BoardIterator::pointer
  //  - a pointer to this->elem
  //

  return &this->elem;
}

BoardIterator &BoardIterator::operator++()
{
  //
  // BoardIterator operator++
  // Increments and returns the iterator
  //
  // returns
  // -------
  // BoardIterator &
  //  - a reference *this
  // 

  ++this->board_itr;
  this->elem.val=*this->board_itr;
  ++this->elem.pos;
  this->elem.ind=static_cast<size_t>(this->elem.pos);
  return *this;
}

BoardIterator BoardIterator::operator++(int)
{
  //
  // BoardIterator operator++(int)
  // Increments the iterator and returns a copy of the pre-incremented iterator
  //
  // returns
  // -------
  // BoardIterator &
  //  - a copy of *this before incrementing
  //

  BoardIterator res=*this;
  ++*this;
  return res;
}

Board::Board() : plays(ITERATION::kZero), cross_count(0), nought_count(0)
{
  //
  // Board class constructor
  // this->plays is set to ITERATION::kZero (0), this->cross_count and this->nought_count are both set to 0, and this->board is filled
  // with VALUE::kEmpty (0)
  //

  std::fill(std::begin(this->board),std::end(this->board),VALUE::kEmpty);
}

Board::Board(const Board &other) : plays(other.plays), cross_count(other.cross_count), nought_count(other.nought_count)
{
  //
  // Board class copy constructor
  // this->plays is set to other.plays, and other.board is copied into this->board
  //
  // parameters
  // ----------
  // other : const Board &
  //  - the Board class object from which to produce a copy
  //

  std::copy(std::cbegin(other.board),std::cend(other.board),std::begin(this->board));
}

Board::Board(Board &&other) : Board()
{
  //
  // Board class move constructor
  // other is moved into *this
  //
  // parameters
  // ----------
  // other : Board &&
  //  - the Board class object to move from
  //

  swap(*this,other);
}

Board::~Board()
{
  //
  // Board class destructor
  //
}

void Board::assign_position(POSITION pos, const VALUE val)
{
  //
  // Set a position on the board
  // Sets given this->board position to given value and increments this->plays
  //
  // parameters
  // ----------
  // pos : POSITION
  //  - the position on the board
  // val : const VALUE
  //  - the value of be set
  //
  // throws
  // ------
  // assigning_empty_value
  //  - if val is VALUE::kEmpty (0)
  // non_empty_position
  //  - if pos is already filled (i.e., does not equal VALUE::kEmpty (0))
  // game_finished
  //  - if the game has finished (by win or draw)
  //

  if (val==VALUE::kEmpty) { throw assigning_empty_value(); }
  if ((*this)[pos]!=VALUE::kEmpty) { throw non_empty_position(pos,(*this)[pos]); }
  if (RESULT res;(res=this->check_board())!=RESULT::kNotWon) { throw game_finished(res); }
  
 (*this)[pos]=val;
  ++this->plays;
  if (val==VALUE::kCross) { ++this->cross_count; }
  else { ++this->nought_count; }
}

void Board::clear_board(void)
{
  //
  // Clear the board
  // Sets all values of this->board to VALUE::kEmpty (0)
  //

  std::fill(std::begin(this->board),std::end(this->board),VALUE::kEmpty);
  this->plays=ITERATION::kZero;
}

RESULT Board::check_board(void) const
{
  //
  // Check to see if the board contains a winning combination
  // Considers if enough moves have been made; if any of the rows, columns, or diagonals are the same; and if all moves have been made
  //
  // returns
  // -------
  // RESULT
  //  - if the board is a draw (RESULT::kDraw (3)), if it has not yet been won (RESULT::kNotWon (0)), if it has been won by cross
  //    (RESULT::kCross (1)), or if it has been won by naught (RESULT::kNought (3))
  //

  if (this->plays<ITERATION::kFive) { return RESULT::kNotWon; } // no one can have won yet

  // check each of the possible winning 3-tuples of positions to see if they are all equal
  for (const std::array<POSITION,3> &itr:kWinningPositions)
  {
    if (this->check_equal(itr)) { return static_cast<RESULT>((*this)[itr[0]]); }
  }
  // if all moves have been played, and no one was found to have won, it is a draw
  if (this->plays==ITERATION::kNine) { return RESULT::kDraw; }
  // otherwise, no one has one yet
  return RESULT::kNotWon;
}

bool Board::check_equal(const std::array<POSITION,3> &pos) const
{
  //
  // Check to see if three given positions equal the same non-empty value
  // Considers if all three positions are non-empty (not VALUE::kEmpty (0)) and if all three positions are equal
  //
  // parameters
  // ----------
  // pos : const std::array<POSITION,3> &
  //  - an array of 3 positions on the board
  //    the method does not check if these positions are contiguous
  //
  // returns
  // -------
  // bool
  //  - true if all positions of this->board, indexed by pos, are the same and non-empty (not VALUE::kEmpty (0)); false otherwise
  //

  // extract the values in the given positions
  std::array<VALUE,3> subset;
  for (int itr=0;itr<3;++itr) { subset[itr]=(*this)[pos[itr]]; }

  if (
    // if none of the array of VALUE::kEMpty (0)
      std::none_of(std::cbegin(subset),std::cend(subset),[](VALUE val){ return val==VALUE::kEmpty; }) &&
    // and there is only one (implicitly, not VALUE::kEmpty (0)) unique element
      std::set<VALUE>(std::cbegin(subset),std::cend(subset)).size()==1)
  // they are equal
  { return true; }
  return false;
}

[[nodiscard]] BoardIterator Board::begin(void) const
{
  //
  // Create an iterator to the beginning of the board
  //
  // returns
  // -------
  // BoardIterator
  //  - an iterator at the beginning of the board
  //

  return BoardIterator(this->board.begin(),this->board.begin());
}

[[nodiscard]] BoardIterator Board::end(void) const
{
  //
  // Create an iterator to the end of the board
  //
  // returns
  // -------
  // BoardIterator
  //  - an iterator at the end of the board
  //

  return BoardIterator(this->board.begin(),this->board.end());
}

[[nodiscard]] ITERATION Board::get_plays(void) const
{
  //
  // Board::plays getter
  //
  // returns
  // -------
  // ITERATION
  //  - the number of moves made on the board
  //

  return this->plays;
}

[[nodiscard]] const VALUE &Board::at(const POSITION &pos)
{
  //
  // Board::board value getter
  // Get a const reference to the value in the specified position
  //
  // parameters
  // ----------
  // pos : const POSITION &
  //  - the position of Board::board
  //
  // returns
  // -------
  // const VALUE &
  //  - the value of Board::board at pos
  //

  return (*this)[pos];
}

[[nodiscard]] const VALUE &Board::at(const POSITION &pos) const
{
  //
  // Board::board value getter
  // Get a const reference to the value in the specified position
  //
  // parameters
  // ----------
  // pos : const POSITION &
  //  - the position of Board::board
  //
  // returns
  // -------
  // const VALUE &
  //  - the value of Board::board at pos
  //

  return (*this)[pos];
}

[[nodiscard]] VALUE &Board::at(POSITION &pos)
{
  //
  // Board::board value getter
  // Get a reference to the value in the specified position
  //
  // parameters
  // ----------
  // pos : POSITION &
  //  - the position of Board::board
  //
  // returns
  // -------
  // VALUE &
  //  - the value of Board::board at pos
  //

  return (*this)[pos];
}

[[nodiscard]] const VALUE &Board::operator[](const POSITION &pos)
{
  //
  // Board operator[]
  // Get a const reference to the value in the specified position
  //
  // parameters
  // ----------
  // pos : const POSITION &
  //  - the position of Board::board
  //
  // returns
  // -------
  // const VALUE &
  //  - the value of Board::board at pos
  //
  // throws
  // ------
  // std::out_of_range
  //  - if pos is out of range
  //

  if (!this->check_range(pos)) { throw std::out_of_range("Indexed position is out of range."); }

  return this->board[static_cast<int>(pos)];
}

[[nodiscard]] const VALUE &Board::operator[](const POSITION &pos) const
{
  //
  // Board operator[]
  // Get a const reference to the value in the specified position
  //
  // parameters
  // ----------
  // pos : const POSITION &
  //  - the position of Board::board
  //
  // returns
  // -------
  // const VALUE &
  //  - the value of Board::board at pos
  //
  // throws
  // ------
  // std::out_of_range
  //  - if pos is out of range
  //

  if (!this->check_range(pos)) { throw std::out_of_range("Indexed position is out of range."); }

  return this->board[static_cast<int>(pos)];
}

[[nodiscard]] VALUE &Board::operator[](POSITION &pos)
{
  //
  // Board operator[]
  // Get a reference to the value in the specified position
  //
  // parameters
  // ----------
  // pos : POSITION &
  //  - the position of Board::board
  //
  // returns
  // -------
  // VALUE &
  //  - the value of Board::board at pos
  //
  // throws
  // ------
  // std::out_of_range
  //  - if pos is out of range
  //

  if (!this->check_range(pos)) { throw std::out_of_range("Indexed position is out of range."); }

  return this->board[static_cast<int>(pos)];
}

bool Board::check_range(const POSITION pos) const
{
  //
  // Check if a given position is in range for Board::board
  // The position is out of range if its integral value is strictly less than 0 or strictly greater than 8
  //
  // parameters
  // ----------
  // pos : const POSITION
  //  - the position to check against
  //
  // returns
  // -------
  // bool
  //  - false if the position is out of range, true otherwise
  //

  if (pos<0||pos>8) { return false; }
  return true;
}

Board &Board::operator=(Board other)
{
  //
  // Board operator=
  // Assign a Board object to the Board in *this using copy-and-swap
  //
  // parameters
  // ----------
  // other : Board
  //  - the Board object to assign from
  //
  // returns
  // -------
  // Board &
  //  - the Board in other now in *this
  //

  swap(*this,other);
  return *this;
}

Board &Board::operator>>=(const int n)
{
  //
  // Board operator>>=
  // Reflect Board::board along its vertical axis.
  //
  // parameters
  // ----------
  // n : const int
  //  - number of reflections to make
  //    if n%2==0, there is no reflection; if n%2==1, there is one reflection
  //
  // returns
  // -------
  // Board &
  //  - reflected board
  //

  if (n%2==1)
  {
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;pos+=3)
    {
      std::swap((*this)[pos],const_cast<VALUE &>((*this)[pos+2]));
    }
  }
  return *this;
}

Board &Board::operator<<=(int n)
{
  //
  // Board operator<<=
  // rotate Board::board n quater turns counterclockwise
  //
  // parameters
  // ----------
  // n : int
  //  - number of turns to make
  //
  // returns
  // -------
  // Board &
  //  - rotated board
  //

  // actual number of turns to make
  n%=4;
  for (int itr=0;itr<n;++itr)
  {
    Board tmp=*this;
    for (POSITION pos=POSITION::kTopLeft;pos<=POSITION::kBottomRight;++pos)
    {
      (*this)[pos]=tmp[(pos+1)*3%10-1]; // if you write it out and do the math, this will make sense
    }
  }

  return *this;
}

void swap(BoardIterator& first, BoardIterator& second) noexcept
{
  //
  // Swap two BoardIterator objects
  // Swaps BoardIterator::board_itr, BoardIterator::begin, and BoardIterator::elem of the two objects
  //
  // parameters
  // ----------
  // first,second : BoardIterator&
  //  - the BoardIterator objects to swap
  //

  std::swap(first.board_itr,second.board_itr);
  std::swap(first.elem,second.elem);
}

void swap(Board &first, Board &second) noexcept
{
  //
  // Swap two Board objects
  // Swaps Board::board, Board::plays, Board::cross_count, and Board::nought_count
  //
  // parameters
  // ----------
  // first,second : Board&
  //  - the Board objects to swap
  //

  std::swap(first.board,second.board);
  std::swap(first.plays,second.plays);
  std::swap(first.cross_count,second.cross_count);
  std::swap(first.nought_count,second.nought_count);
}
} // namespace menace
