
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// control.cpp                                                                                                                           //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Implementation of the Control class of MENACE


#include <control.hpp>

namespace menace
{
Control::Control() :
  board(Board()), ui(UI(&board)), human_one(Player(VALUE::kCross,board)), human_two(Player(VALUE::kNought,board)),
  menace_one(MENACE(VALUE::kCross,board)), menace_two(MENACE(VALUE::kNought,board)),
  random_one(Random(VALUE::kCross,board)), random_two(Random(VALUE::kNought,board)),
  automaton_one(nullptr), automaton_two(nullptr)
{
  //
  // Control class constructor
  //
}

Control::~Control()
{
  //
  // Control class destructor
  //
}

void Control::run(void)
{
  //
  // Controller of game execution
  //

  while (true)
  {
    size_t argc=0;
    std::vector<std::string> argv=ui.get_command(argc);

    // if enter is pressed
    if (argc==0) { continue; }
    // display the help
    if (argv[0]=="help")
    {
      // incorrect number of arguments
      if (!(argc==1||argc==2))
      {
        ui.print_error(ERR::kInvalidArgumentNumber,{argv[0],std::to_string(argc-1)});
      }
      // requested general help
      else if (argc==1)
      {
        ui.print_help();
      }
      // requested specific help
      else if (argv[1]=="play")
      {
        ui.print_help(HELP::kPlay);
      }
      else if (argv[1]=="reset")
      {
        ui.print_help(HELP::kReset);
      }
      // requested help of a legitimate command but which has no additional help
      else if (find(kUnhelpableCommands,argv[1])!=-1)
      {
        ui.print_error(ERR::kInvalidHelpArgument,{argv[1]});
      }
      // requested help of a command which doesn't exist
      else
      {
        ui.print_error(ERR::kInvalidArguments,argv);
      }
    }
    // play a game
    else if (argv[0]=="play")
    {
      // incorrect number of arguments
      if (!(argc==1||argc==2||argc==3))
      {
        ui.print_error(ERR::kInvalidArgumentNumber,{argv[0],std::to_string(argc-1)});
        continue; // control loop
      }
      std::unique_ptr<Game> game;
      try
      {
        game=this->parse_game(argc==1 ? std::vector<std::string>{"menace"} : std::vector<std::string>(std::begin(argv)+1,std::end(argv)));
      }
      // an unkown game was given
      catch(const unknown_play_option &err)
      {
        ui.print_error(ERR::kInvalidArguments,std::vector<std::string>{argv[0]}+err.option);
        continue; // control loop
      }
      WINDOWS::COORD origin=ui.origin();

      ui.print_board();
      // print game help if a human is playing
      if (find(kHumanIncludedGames,game->type)!=-1)
      {
        ui.move(0,7,origin); // move back to user input location
        ui.print_play_help();
      }
      ui.move(0,7,origin); // move back to user input location

      // some bools to not have to calculate on each iteration of the game
      const bool automaton_first_game=find(kAutomatonFirstGames,game->type)!=-1
        ,automaton_second_game=find(kAutomatonSecondGames,game->type)!=-1
        ,meance_first_game=find(kMENACEFirstGames,game->type)!=-1,menace_second_game=find(kMENACESecondGames,game->type)!=-1;
      bool exit_game=false;

      // play game
      while (board.check_board()==RESULT::kNotWon)
      {
        // first: determine move to make
        POSITION pos;
        if (game->get_plays()%2==0&&automaton_first_game)
        {
          if (meance_first_game) { ui.print_menace(); }
          pos=automaton_one->get_move();
        }
        else if (game->get_plays()%2==1&&automaton_second_game)
        {
          if (menace_second_game) { ui.print_menace(); }
          pos=automaton_two->get_move();
        }
        // get a move from the player
        else
        {
          try
          {
            pos=ui.get_move(game->get_plays());
          }
          catch (const unknown_position &err)
          {
            ui.clear(0,8,77,origin); // clear the error location
            ui.move(0,8,origin); // move to error location
            ui.print_error(ERR::kUnknownPosition,{err.pos}); // print the error
            ui.clear(30,7,77,origin); // clear user input
            ui.move(0,7,origin); // move to user input location
            continue;  // play loop
          }
          catch (const early_exit &)
          {
            exit_game=true;
          }
          catch (const user_play_request &req)
          {
            switch(req.request)
            {
            case (REQUEST::kAdvise):
            {
              // get a move from MENACE
              Board advise_board=this->board;
              ui.display_board=&advise_board;
              MENACE advise_menace=MENACE(game->players[static_cast<int>(game->get_plays()%2)]->value,advise_board);
              POSITION advise_pos=advise_menace.get_move();
              // make the move on the temporary board
              advise_board.assign_position(advise_pos,advise_menace.value);

              // mark that position as adivisial
              ui.set_advisial(advise_pos);

              ui.clear(77); // clear beneath the user input
              ui.move(0,0,origin); // move back to origin
              ui.print_board();
              ui.clear(30,7,77,origin); // clear user input
              ui.move(0,7,origin); // move to user input location

              // reset boards back
              ui.display_board=&board;
              ui.reset_advisial();
              continue; // play loop
            }
            case (REQUEST::kMove):
            {
              // get a move from MENACE
              Board move_board(*ui.display_board);
              MENACE move_menace=MENACE(game->players[static_cast<int>(game->get_plays()%2)]->value,move_board);
              POSITION move_pos=move_menace.get_move();
              // make the move
              game->play(move_pos);

              ui.clear(0,8,77,origin); // clear beneath the user input
              ui.clear(30,7,77,origin); // clear user input
              ui.move(0,7,origin); // move to user input location

              continue; // play loop
            }
            }
          }
        }
        if (exit_game) { break; }

        // second: play move
        try
        {
          game->play(pos);
        }
        catch(const non_empty_position &err)
        {
          ui.clear(77); // clear beneath the user input
          ui.move(0,8,origin); // move to error location
          ui.print_error(ERR::kInvalidPosition,{kPositionStrings.at(err.pos),kValueStrings.at(err.val)});
          ui.clear(30,7,77,origin); // clear user input
          ui.move(0,7,origin); // move to user input location
          continue; // play loop
        }
        catch(const game_finished &)
        {
          break;
        }

        // third: reset for next move
        ui.clear(77); // clear beneath the user input
        ui.move(0,0,origin); // move back to origin
        ui.print_board();
        ui.clear(30,7,77,origin); // clear user input
        ui.move(0,7,origin); // move to user input location
      }
      if (exit_game)
      {
        // reset for the next game
        automaton_one=nullptr;
        automaton_two=nullptr;
        board.clear_board();
        // print play help
        if (find(kHumanIncludedGames, game->type)!=-1)
        {
          ui.clear(CLEAR::kPlayHelp,origin);
        }
        ui.clear(0,7,77,origin); // clear user input
        ui.clear(0,8,77,origin); // clear errors
        ui.move(0,7,origin); // move to user input location
        continue; // control loop
      }

      ui.clear(0,7,77,origin); // clear user input
      ui.move(0,7,origin); // move to user input location
      ui.print_result(board.check_board(),game->type);

      // print play help
      if (find(kHumanIncludedGames,game->type)!=-1)
      {
        ui.clear(CLEAR::kPlayHelp,origin);
        ui.move(0,9,origin); // move to user input location
      }

      // reset for the next game
      automaton_one=nullptr;
      automaton_two=nullptr;
      board.clear_board();
    }
    // reset menace's transposition table
    else if (argv[0]=="reset")
    {
      // incorrect number of arguments
      if (!(argc==1||argc==2))
      {
        ui.print_error(ERR::kInvalidArgumentNumber,{argv[0],std::to_string(argc-1)});
      }
      // clear both tables
      else if (argc==1)
      {
        menace_one.clear_tansposition_table();
        menace_two.clear_tansposition_table();
      }
      // clear a specific table
      else if (argc==2)
      {
        if (argv[1]=="one") { menace_one.clear_tansposition_table(); }
        else if (argv[1]=="two") { menace_two.clear_tansposition_table(); }
        // incorrect additional command
        else { ui.print_error(ERR::kInvalidArguments,argv);}
      }
    }
    // clear the screen
    else if (argv[0]=="cls")
    {
      // incorrect number of arguments
      if (argc!=1) { ui.print_error(ERR::kInvalidArgumentNumber,{argv[0],std::to_string(argc-1)}); }
      ui.clear();
    }
    // exit the game
    else if (argv[0]=="exit"||argv[0]=="quit"||argv[0]=="q")
    {
      // incorrect number of arguments
      if (argc!=1) { ui.print_error(ERR::kInvalidArgumentNumber,{argv[0],std::to_string(argc-1)}); }
      ui.print_exit();
      return;
    }
    // command unkown
    else
    {
      ui.print_error(ERR::kUnkownCommand,{argv[0]});
    }
  }
}

std::unique_ptr<Game> Control::parse_game(const std::vector<std::string> &game_type)
{
  //
  // Creates a Game instance from user specifications
  // Does a shameful check through all possible combinations to return the correct Game instance
  //
  // parameters
  // ----------
  // game_type : const std::vector<std::string> &
  //  - a vector of strings decribing the game to be played
  //
  // returns
  // -------
  // std::unique_ptr<Game>
  //  - a new game instance
  //    the Game is not managed by this function. the caller is responsible for deleting it
  //
  // throws
  // ------
  // unknown_play_option
  //  - if the strings in game_type do not describe a valid game
  //

  const size_t size=game_type.size();
  std::unique_ptr<Game> game;
  // again, this is pretty shameful
  if (size==1&&(game_type[0]=="pvp"||game_type[0]=="person")||size==2&&game_type[0]=="person"&&game_type[1]=="person")
  {
    game=std::make_unique<Game>(board,&human_one,&human_two,GAME::kPvP);
  }
  else if (
    size==1&&(game_type[0]=="pvm"||game_type[0]=="menace")||size==2&&game_type[0]=="menace"&&game_type[1]=="second"||
    size==2&&game_type[0]=="person"&&game_type[1]=="menace"
  )
  {
    automaton_two=&menace_two;
    game=std::make_unique<Game>(board,&human_one,&menace_two,GAME::kPvM);
  }
  else if (
    size==1&&game_type[0]=="mvp"||size==2&&game_type[0]=="menace"&&game_type[1]=="first"||
    size==2&&game_type[0]=="menace"&&game_type[1]=="person"
  )
  {
    automaton_one=&menace_one;
    game=std::make_unique<Game>(board,&menace_one,&human_two,GAME::kMvP);
  }
  else if (size==1&&game_type[0]=="mvm"||size==2&&game_type[0]=="menace"&&(game_type[1]=="itself"||game_type[1]=="menace"))
  {
    automaton_one=&menace_one;
    automaton_two=&menace_two;
    game=std::make_unique<Game>(board,&menace_one,&menace_two,GAME::kMvM);
  }
  else if (size==1&&game_type[0]=="rvr"||size==2&&game_type[0]=="random"&&(game_type[1]=="itself"||game_type[1]=="random"))
  {
    automaton_one=&random_one;
    automaton_two=&random_two;
    game=std::make_unique<Game>(board,&random_one,&random_two,GAME::kRvR);
  }
  else if (size==1&&game_type[0]=="mvr"||size==2&&game_type[0]=="menace"&&game_type[1]=="random")
  {
    automaton_one=&menace_one;
    automaton_two=&random_two;
    game=std::make_unique<Game>(board,&menace_one,&random_two,GAME::kMvR);
  }
  else if (size==1&&game_type[0]=="rvm"||size==2&&game_type[0]=="random"&&game_type[1]=="menace")
  {
    automaton_one=&random_one;
    automaton_two=&menace_two;
    game=std::make_unique<Game>(board,&random_one,&menace_two,GAME::kRvM);
  }
  else if (
    size==1&&game_type[0]=="rvp"||size==2&&game_type[0]=="random"&&game_type[1]=="first"||
    size==2&&game_type[0]=="random"&&game_type[1]=="person"
  )
  {
    automaton_one=&random_one;
    game=std::make_unique<Game>(board,&random_one,&human_two,GAME::kRvP);
  }
  else if (
    size==1&&(game_type[0]=="pvr"||game_type[0]=="random")||size==2&&game_type[0]=="random"&&game_type[1]=="second"||
    size==2&&game_type[0]=="person"&&game_type[1]=="random"
  )
  {
    automaton_two=&random_two;
    game=std::make_unique<Game>(board,&human_one,&random_two,GAME::kPvR);
  }
  else { throw(unknown_play_option(game_type)); }
  return game;
}
} // namespace menace
