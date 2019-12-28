
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// player.cpp                                                                                                                            //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Implementation of the Player classes of MENACE


#include <player.hpp>
#include <iostream>

namespace menace
{
Player::Player(const VALUE value, Board &board) : value(value), board(board)
{
  //
  // Player class constructor
  // this->value and this->board are set to corresponding inputs
  //
  // parameters
  // ----------
  // value : const VALUE
  //  - the value for the Player to play
  // board : Board &
  //  - the board for the Player to play on
  //
  // throws
  // ------
  // assigning_empty_player
  //  - if value is the same as VALUE::kEmpty (0)
  //

  if (value==VALUE::kEmpty) { throw assigning_empty_player(); }
}

void Player::make_move(POSITION pos) const
{
  //
  // Make a move at a given position
  //
  // parameters
  // ----------
  // pos : POSITION  
  //  - the position to play on
  //

  this->board.assign_position(pos,this->value);
}

[[nodiscard]] PLAYER Player::ident(void) const
{
  return PLAYER::kPlayer;
}

Automaton::Automaton(const VALUE value, Board &board) : Player(value,board)
{
  //
  // Automaton class constructor
  //
  // parameters
  // ----------
  // value : const VALUE
  //  - the value for the Automaton to play
  // board : Board &
  //  - the board for the Automaton to play on
  //
}

bool Automaton::valid_move(const Board &state, POSITION pos) const
{
  //
  // Determine if making a move is valid on a given board
  // Checks that the given board at the given position is not VALUE::kEmpty (0)
  //
  // parameters
  // ----------
  // state : const Board & 
  //  - the Board to check against
  // pos : POSITION 
  //  - the position of the move
  //

  return state[pos]==VALUE::kEmpty;
}

bool Automaton::valid_move(const POSITION pos) const
{
  //
  // Determine if making a move is valid on Player::board
  // Checks that this->board at the given position is not VALUE::kEmpty (0)
  //
  // parameters
  // ----------
  // pos : POSITION 
  //  - the position of the move
  //

  return this->board[pos]==VALUE::kEmpty;
}

std::vector<POSITION> Automaton::determine_possible_moves(const Board &state) const
{
  //
  // Determine all possible moves that can be made on a given board
  //
  // parameters
  // ----------
  // state : const Board & 
  //  - the Board to check against
  //
  // returns
  // -------
  // std::vector<POSITION>
  //  - all positions where a move can be made on state
  //

  std::vector<POSITION> possible;
  for (const BoardElement &itr:state)
  {
    if (itr.val==VALUE::kEmpty) { possible.push_back(itr.pos); }
  }

  return possible;
}

std::vector<POSITION> Automaton::determine_possible_moves(void) const
{
  //
  // Determine all possible moves that can be made on Player::board
  //
  // returns
  // -------
  // std::vector<POSITION>
  //  - all positions where a move can be made on Player::board
  //

  return this->determine_possible_moves(this->board);
}

MENACE::MENACE(const VALUE value, Board &board) : Automaton(value,board)
{
  //
  // MENACE class constructor
  //
  // parameters
  // ----------
  // value : const VALUE
  //  - the value for MENACE to play
  // board : Board &
  //  - the board for MENACE to play on
  //
}

POSITION MENACE::get_move(void)
{
  //
  // Get a move from MENACE
  // Uses minimax to determine the value of all possible moves and randomly chooses one of the moves which returns the best value
  //
  // returns
  // -------
  // POSITION
  //  - the position for MENACE to make
  //

  std::array<int,9> values;
  // -1 is the worst MENACE can be gauranteed, so fill with minimum integer so there is a way to distinguish invalid moves and valid moves 
  // with value -1
  std::fill(std::begin(values),std::end(values),std::numeric_limits<int>::min());

  std::vector<POSITION> possible=this->determine_possible_moves();

  // consider all possible positions
  for (POSITION pos:possible)
  {
    // create the child
    Board child=this->board;
    child.assign_position(pos,this->value);
    // determine its value, the MENACE in *this is the maximising player
    int val=this->minimax(child,-1,1,false,this->value,this->value==VALUE::kCross ? VALUE::kNought : VALUE::kCross);
    values[static_cast<int>(pos)]=val;
  }

  // find the maximum value
  const std::array<int,9>::const_iterator max=std::max_element(std::cbegin(values),std::cend(values));
  // push the positions which enable the maximum into out
  std::vector<POSITION> out;
  for (std::array<int,9>::const_iterator itr=std::cbegin(values),end=std::cend(values);itr!=end;++itr)
  {
    if (*itr==*max) { out.push_back(static_cast<POSITION>(std::distance(std::cbegin(values),itr))); }
  }
  // randomise and return
  std::shuffle(std::begin(out),std::end(out),std::mt19937_64{std::random_device{}()});
  return out.front();
}

int MENACE::minimax(const Board &state, int alpha, int beta, const bool maximising_player, const VALUE maximiser_value
  , const VALUE minimiser_value)
{
  //
  // Determine value of a given board
  // Recursively applies minimax to all child boards of the input board to determine its value
  // Uses alpha-beta pruning to reduce the search tree
  //
  // parameters
  // ----------
  // state : const Board &
  //  - the Board to evaluate
  // alpha : int
  //  - the worst value the maximising player can be gauranteed
  // beta : int
  //  - the worst value the minimising player can be gauranteed
  // maximising_player : const bool
  //  - if the player calling minimax seeks to maximise the value they obtain
  // maximiser_value : const VALUE
  //  - the VALUE of the player maximising the output of the board
  // minimiser_value : const VALUE
  //  - the VALUE of the player minimising the output of the board
  //
  // returns
  // -------
  // int
  //  - the value of the input board
  //

  // board has already been evaluated
  if (int n;(n=find(this->evaluated_boards,state))!=-1)
  {
    return this->evaluations[n];
  }
  // board has been won
  if (RESULT res;(res=state.check_board())!=RESULT::kNotWon)
  {
    // 0 if draw, 1 if won, -1 if lost
    int val=res==RESULT::kDraw ? 0 : res==static_cast<RESULT>(maximiser_value) ? 1 : -1;
    // save the board and its value
    this->evaluated_boards.push_back(state);
    this->evaluations.push_back(val);
    return val;
  }
  // else consider all children states of the current state
  std::vector<POSITION> possible=this->determine_possible_moves(state);

  int val=maximising_player ? -1 : 1; // worst the player can be gauranteed
  const bool maximising_child=!maximising_player;
  const VALUE player_val=maximising_player ? maximiser_value : minimiser_value;

  const std::function<const int &(const int &, const int &)> comparator=maximising_player ?
    static_cast<const int &(*)(const int &, const int &)>(&std::max<int>) :
    static_cast<const int &(*)(const int &, const int &)>(&std::min<int>);
  int &pruner=maximising_player ? beta : alpha;
  
  // consider all child states
  for (POSITION pos:possible)
  {
    // create the child
    Board child=state;
    child.assign_position(pos,player_val);
    // determine its value
    val=comparator(this->minimax(child,alpha,beta,maximising_child,maximiser_value,minimiser_value),val);
    pruner=comparator(pruner,val);
    // the decision tree can be pruned
    if (alpha>=beta) { break; }
  }
  // save the board and its value
  this->evaluated_boards.push_back(state);
  this->evaluations.push_back(val);
  return val;
}

void MENACE::clear_tansposition_table(void)
{
  //
  // Clear both this->evaluated_boards and this->evaluations
  //

  this->evaluated_boards.clear();
  this->evaluations.clear();
}

[[nodiscard]] PLAYER MENACE::ident(void) const
{
  return PLAYER::kMENACE;
}

Random::Random(const VALUE value, Board &board) : Automaton(value,board)
{
  //
  // Random class constructor
  //
  // parameters
  // ----------
  // value : const VALUE
  //  - the value for the Random player to play
  // board : Board &
  //  - the board for the Random player to play on
  //
}

POSITION Random::get_move(void)
{
  //
  // Get a move from a Random player
  //
  // returns
  // -------
  // POSITION
  //  - the position for Random to make
  //

  // construct all possile moves, shuffle, and return
  std::vector<POSITION> possible=this->determine_possible_moves();
  std::shuffle(std::begin(possible),std::end(possible),std::mt19937_64{std::random_device{}()});
  return possible.front();
}

[[nodiscard]] PLAYER Random::ident(void) const
{
  return PLAYER::kRandom;
}
} // namespace menace
