
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//                                                                                                                                       //
// ui.cpp                                                                                                                                //
//                                                                                                                                       //
// D. C. Groothuizen Dijkema - November, 2019                                                                                            //
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

// Implementation of the UI class of MENACE


#include <ui.hpp>

namespace menace
{
// save a global handle for console manipulation
const WINDOWS::HANDLE UI::std_handle=WINDOWS::GetStdHandle(STD_OUTPUT_HANDLE);
// no positions begin as advisial
std::array<bool,9> UI::advisial{{false,false,false,false,false,false,false,false,false}};

UI::UI(const Board *display_board) : display_board(display_board)
{
  //
  // UI class constructor
  // Assign this->display_board, fill UI::advisial, and print the intro screen
  //
  // parameters
  // ----------
  // display_board : const Board *
  //  - the game's board, to be printed with
  //

  this->intro_screen();
}

UI::~UI()
{
  //
  // UI class destructor
  //
}

void UI::intro_screen(void)
{
  // 
  // Clear console and print introductory message
  //

  system("cls");
  std::cout
    << std::setfill('-') << std::setw(76) << "" << "\n" << std::setfill(' ')
    << colour_text("Welcome to MENACE: the Machine Educable Noughts and Crosses Engine",COLOUR::kRed) << "\n"
    << colour_text("Type 'help' for help and more information.",COLOUR::kCyan) << "\n"
    << std::setfill('-') << std::setw(76) << "" << "\n" << std::setfill(' ');
  std::cout.flush();
}

void UI::topic_screen(void)
{
  //
  // Clear console and print message
  //

  system("cls");
  std::cout 
    << std::setfill('-') << std::setw(76) << "" << "\n" << std::setfill(' ')
    << colour_text("MENACE: the Machine Educable Noughts and Crosses Engine",COLOUR::kRed) << "\n"
    << std::setfill('-') << std::setw(76) << "" << "\n" << std::setfill(' ');
  std::cout.flush();
}

std::vector<std::string> UI::get_command(size_t &argc)
{
  //
  // Get command from user
  // Get command and parse into a std::vector, saving the number of arguments
  //
  // parameters
  // ----------
  // argc : size_t &
  //  - number of arguments
  //
  // returns
  // -------
  // std::vector<std::string>
  //  - all user's commands
  //

  std::string command;
  
  std::cout << ">> ";
  std::getline(std::cin,command);

  std::vector<std::string> argv;
  size_t found,itr=0;
  // loop over the input to split along a space
  while ((found=command.find(" ",itr))!=std::string::npos)
  {
    argv.push_back(command.substr(itr,found-itr));
    itr=found+1;
  }
  argv.push_back(command.substr(itr));

  // get rid of all args which are just white space
  argv.erase(std::remove_if(std::begin(argv),std::end(argv)
    ,[](const std::string &s){ return std::all_of(std::cbegin(s),std::cend(s),[](char c){ return std::isspace(c); }); }),std::end(argv));
  argc=argv.size();

  // if there was only one empty argument, reset both
  if (argc==1&&argv[0]=="")
  {
    argc=0;
    argv.clear();
  }
  // convert all args to lower case
  for (std::string &arg:argv)
  {
    boost::algorithm::to_lower(arg);
  }
  return argv;
}

POSITION UI::get_move(const ITERATION itr)
{
  //
  // Get move from user
  // Get position and turn the various forms of acceptable input into a valid POSITION
  //
  // parameters
  // ----------
  // itr : const ITERATION
  //  - the current move the game is on, to determine which user to prompt
  //
  // returns
  // -------
  // POSITION
  //  - where the user will move
  //
  // throws
  // ------
  // early_exit
  //  - if the user enters 'exit', 'break', 'quit', or 'q'
  // unkown_position
  //  - if the user specifies an invalid position
  //

  // determine whose play it is
  std::string player(itr%2==0 ? "One (X)" : "Two (O)");
  // prompt
  std::cout << ">> " << colour_text("Player "+player+", make your move: ",COLOUR::kYellow);

  // get desired location
  std::string pos;
  std::getline(std::cin,pos);
  std::string raw=pos; // save raw input for error throw
  // remove white space
  pos.erase(std::remove_if(pos.begin(),pos.end(),::isspace),pos.end());
  // if the user is requesting advice
  if (std::map<std::string,REQUEST>::const_iterator req;(req=kUserRequests.find(pos))!=std::cend(kUserRequests))
  {
    throw user_play_request(req->second);
  }
  // if the user wants to quit the game (coward)
  if (pos=="quit"||pos=="q"||pos=="break"||pos=="exit") { throw early_exit(); }

  // an actual input must be at least 2 characters
  if (pos.length()==1) { throw unknown_position(raw); }

  // check first if the position is given as two numbers
  int row,col;
  try
  {
    row=std::stoi(pos.substr(0,1));
    col=std::stoi(pos.substr(pos.size()-1,1));
    if ((find(std::vector<int>({0,1,2}),row)==-1)||(find(std::vector<int>({0,1,2}),col)==-1)) { throw unknown_position(raw); } // invalid int
    return static_cast<POSITION>(row*3+col); // valid int
  }
  // if the above fails, the position was entered as words
  catch(...) {}

  // convert to lower case
  boost::algorithm::to_lower(pos);

  // unfortunately, this is just going to have to be a bit brute-forcey:
  if (pos.substr(0,4)=="left"){ pos=pos.substr(4)+pos.substr(0,4); } // move 'left' to the end
  else if (pos.substr(0,5)=="right") { pos=pos.substr(5)+pos.substr(0,5); } // move 'right' to the end
  // move 'middle' to the end if the position begins with 'top' or 'bottom'
  else if (pos.substr(0,6)=="middle"&&find(kRowNames,pos.substr(6))!=-1) { pos=pos.substr(6)+pos.substr(0,6); }

  // else, no processing is needed, or the user has entered something completely wrong
  int n=find(kPositions,pos);
  if (n==-1) { throw unknown_position(raw); }
  return static_cast<POSITION>(n);
}

void UI::print_menace(void)
{
  //
  // Print message to console that MENACE is determining its move
  //

  std::cout << ">> " << colour_text("MENACE is determining its move.",COLOUR::kYellow);
  std::cout.flush();
}

void UI::print_help(void)
{
  //
  // Print the user help to the console
  //

  const int max_width=19,line_gap=7,init_gap=3;

  std::cout << "\n" << blank(init_gap)
    << colour_text("MENACE: the Machine Educable Noughts and Crosses Engine",COLOUR::kRed)
    << "\n" << "\n" << blank(init_gap)
    << colour_text("MENACE",COLOUR::kRed)
    << colour_text(" is a computer algorithm which can play Noughts and Crosses. You",COLOUR::kCyan)
    << "\n" << blank(init_gap)
    << colour_text("can play other people, play ",COLOUR::kCyan) << colour_text("MENACE",COLOUR::kRed)
    << colour_text(", or watch ",COLOUR::kCyan) << colour_text("MENACE",COLOUR::kRed) << colour_text(" play itself. ",COLOUR::kCyan)
    << colour_text("MENACE",COLOUR::kRed)
    << "\n" << blank(init_gap)
    << colour_text("learns fast: *very* fast. One starting move is all ",COLOUR::kCyan)
    << colour_text("MENACE",COLOUR::kRed) << colour_text(" needs to have",COLOUR::kCyan)
    << "\n" << blank(init_gap)
    << colour_text("considered every possible game.",COLOUR::kCyan)
    << "\n" << "\n" << blank(init_gap)
    << colour_text("Copyright (c) David Cornelis Groothuizen Dijkema, 2019",COLOUR::kCyan)
    << "\n" << blank(init_gap)
    << colour_text("This is free software, and you are welcome to redistribute it and/or",COLOUR::kCyan)
    << "\n" << blank(init_gap)
    << colour_text("modify it under the terms of the MIT License.",COLOUR::kCyan)
    << "\n" << "\n" << blank(init_gap)
    << colour_text("For more information on a specifc command, type ",COLOUR::kCyan)
    << colour_text("help <command>",COLOUR::kPurple) << colour_text(".",COLOUR::kCyan)
    << "\n" << "\n" << blank(line_gap) << std::setw(max_width) << std::left
    << colour_text("help",COLOUR::kGreen) << blank(1) << colour_text("display help and more information",COLOUR::kPurple)
    << "\n" << "\n" << blank(line_gap) << std::setw(max_width) << std::left
    << colour_text("play",COLOUR::kGreen) << blank(1) << colour_text("play a game of Noughts and Crosses",COLOUR::kPurple)
    << "\n" << "\n" << blank(line_gap) << std::setw(max_width) << std::left
    << colour_text("reset",COLOUR::kGreen) << blank(1) << colour_text("reset ",COLOUR::kPurple) << colour_text("MENACE's",COLOUR::kRed)
    << colour_text(" transposition tables",COLOUR::kPurple)
    << "\n" << "\n" << blank(line_gap) << std::setw(max_width) << std::left
    << colour_text("cls",COLOUR::kGreen) << blank(1) << colour_text("clear the console",COLOUR::kPurple)
    << "\n" << "\n" << blank(line_gap) << std::setw(max_width) << std::left
    << colour_text("exit",COLOUR::kGreen) << blank(1) << colour_text("terminate the programme",COLOUR::kPurple) << "\n";
  std::cout.flush();
}

void UI::print_play_help(void)
{
  //
  // Print the game play help to the console
  //

  const int init_gap=3,line_gap=5,new_line_gap=line_gap;
  std::cout << "\n" << "\n" << blank(init_gap)
    << colour_text("You can specify your move in either of the following ways:",COLOUR::kCyan)

    << "\n" << "\n" << blank(line_gap)
    << colour_text("As two integers between 0 and 2, representing the row and column you",COLOUR::kCyan)
    << "\n" << blank(new_line_gap)
    << colour_text("wish to play in. For example, ",COLOUR::kCyan) << colour_text("01",COLOUR::kPurple)
    << colour_text(" would move your to the top",COLOUR::kCyan)
    << "\n" << blank(new_line_gap)
    << colour_text("middle, while ",COLOUR::kCyan) << colour_text("21",COLOUR::kPurple) << colour_text(" would move you",COLOUR::kCyan)
    << colour_text(" to the bottom middle. The first number",COLOUR::kCyan)
    << "\n" << blank(new_line_gap)
    << colour_text("is for the row and the second number is for the column.",COLOUR::kCyan)

    << "\n" << "\n" << blank(line_gap)
    << colour_text("As two words, with one representing the row with ",COLOUR::kCyan) << colour_text("top",COLOUR::kPurple)
    << colour_text(", ",COLOUR::kCyan) << colour_text("middle",COLOUR::kPurple) << colour_text(", or ",COLOUR::kCyan)
    << "\n" << blank(new_line_gap)
    << colour_text("bottom",COLOUR::kPurple) << colour_text(", and the other representing the column with ",COLOUR::kCyan)
    << colour_text("left",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan) << colour_text("middle",COLOUR::kPurple)
    << "\n" << blank(new_line_gap)
    << colour_text("or ",COLOUR::kCyan) << colour_text("right",COLOUR::kPurple) << colour_text(". For example ",COLOUR::kCyan) 
    << colour_text("top middle",COLOUR::kPurple)  << colour_text(" or ",COLOUR::kCyan) << colour_text("left bottom",COLOUR::kPurple) 
    << colour_text(". These words do mot",COLOUR::kCyan)
    << "\n" << blank(new_line_gap)
    << colour_text("have to be in any order. Either the column or the row can be specified",COLOUR::kCyan)
    << "\n" << blank(new_line_gap)
    << colour_text("first.",COLOUR::kCyan)

    << "\n" << "\n" << blank(init_gap)
    << colour_text("You cannnot move where another player has already so moved. You can quit",COLOUR::kCyan)
    << "\n" << blank(init_gap)
    << colour_text("the game by entering ",COLOUR::kCyan) << colour_text("break",COLOUR::kPurple)
    << colour_text(". Enter ",COLOUR::kCyan) << colour_text("advise",COLOUR::kPurple) 
    << colour_text(" for a recommended move, or ",COLOUR::kCyan) << colour_text("move",COLOUR::kPurple)
    << "\n" << blank(init_gap)
    << colour_text("to have the best move made for you.",COLOUR::kCyan);
    std::cout.flush();
}

void UI::print_help(const HELP help)
{
  //
  // Print additional help to the console
  //
  // parameters
  // ----------
  // help : const HELP
  //  - the help the user wants
  //

  const int init_gap=3,line_gap=5;

  switch (help)
  {
  case HELP::kPlay:
  {
    std::cout << "\n" << blank(init_gap)
      << colour_text("play",COLOUR::kPurple)
      << "\n" << blank(init_gap)
      << colour_text("play <type>",COLOUR::kPurple)
      << "\n" << blank(init_gap)
      << colour_text("play <opponent>",COLOUR::kPurple)
      << "\n" << blank(init_gap)
      << colour_text("play <opponent> <order>",COLOUR::kPurple)
      << "\n" << blank(init_gap)
      << colour_text("play <opponent> <opponent>",COLOUR::kPurple)
      << "\n" << "\n" << blank(init_gap)
      << colour_text("options",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << std::setfill('-') << std::setw(7) << "" << std::setfill(' ')
      << "\n" << blank(init_gap)
      << colour_text(" : (default) play ",COLOUR::kCyan) << colour_text("MENACE",COLOUR::kRed)
      << colour_text(" and you make the first move.",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("<type>",COLOUR::kPurple) << colour_text(" : (optional) the type of game to play.",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("Must be one of ",COLOUR::kCyan) << colour_text("pvp",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan) 
      << colour_text("pvm",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan) << colour_text("mvp",COLOUR::kPurple) 
      << colour_text(", ",COLOUR::kCyan)<< colour_text("mvm",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan) 
      << colour_text("mvr",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan) << colour_text("rvm",COLOUR::kPurple) 
      << colour_text(", ",COLOUR::kCyan)<< colour_text("rvp",COLOUR::kPurple) << colour_text(", or ",COLOUR::kCyan) 
      << colour_text("pvr",COLOUR::kPurple) << colour_text(",",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("where ",COLOUR::kCyan) << colour_text("p",COLOUR::kPurple) << colour_text(" stands for ",COLOUR::kCyan)
      << colour_text("person",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan) << colour_text("m",COLOUR::kPurple)
      << colour_text(" stands for ",COLOUR::kCyan) << colour_text("MENACE",COLOUR::kRed) << colour_text(", and ",COLOUR::kCyan)
      << colour_text("r",COLOUR::kPurple) << colour_text(" stands for ",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("random",COLOUR::kPurple) << colour_text(".",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("<opponent>",COLOUR::kPurple) << colour_text(" : (optional) who to play against.",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("Must be one of ",COLOUR::kCyan) << colour_text("menace",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan)
      << colour_text("random",COLOUR::kPurple) << colour_text(", or ",COLOUR::kCyan)
      << colour_text("person",COLOUR::kPurple) << colour_text(".",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("<opponent> <order>",COLOUR::kPurple) 
      << colour_text(" : (optional) who to play against and in what order.",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("<opponent>",COLOUR::kPurple) 
      << colour_text(" must be one of ",COLOUR::kCyan) << colour_text("menace",COLOUR::kPurple) << colour_text(", or ",COLOUR::kCyan)
      << colour_text("random",COLOUR::kPurple) << colour_text(".",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("<order>",COLOUR::kPurple) 
      << colour_text(" must be one of ",COLOUR::kCyan) << colour_text("first",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan)
      << colour_text("second",COLOUR::kPurple) << colour_text(", or ",COLOUR::kCyan) << colour_text("itself",COLOUR::kPurple) 
      << colour_text(".",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("<opponent> <opponent>",COLOUR::kPurple)
      << colour_text(" : (optional) who is playing the game, in order.",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("Each must be one of ",COLOUR::kCyan) << colour_text("menace",COLOUR::kPurple) << colour_text(", ",COLOUR::kCyan)
      << colour_text("random",COLOUR::kPurple) << colour_text(", or ",COLOUR::kCyan)
      << colour_text("person",COLOUR::kPurple) << colour_text(".",COLOUR::kCyan)
      << "\n";
    break;
  }
  case HELP::kReset:
  {
    std::cout << "\n" << blank(init_gap)
      << colour_text("reset <index>",COLOUR::kPurple)
      << "\n" << "\n" << blank(init_gap)
      << colour_text("options",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << std::setfill('-') << std::setw(7) << "" << std::setfill(' ')
      << "\n" << blank(init_gap)
      << colour_text("<index>",COLOUR::kPurple) << colour_text(" : (optional) ",COLOUR::kCyan) << colour_text("MENACE",COLOUR::kRed)
      << colour_text(" of which to reset the transposition table.",COLOUR::kCyan)
      << "\n" << blank(line_gap)
      << colour_text("Must be one of ",COLOUR::kCyan) << colour_text("one",COLOUR::kPurple) << colour_text(" or ",COLOUR::kCyan)
      << colour_text("two",COLOUR::kPurple) << colour_text(". Specifying nothing will reset both.",COLOUR::kCyan)
      << "\n" << "\n" << blank(init_gap)
      << colour_text("reset",COLOUR::kPurple) << colour_text(" will clear the transposition table ",COLOUR::kCyan)
      << colour_text("MENACE",COLOUR::kRed) << colour_text(" uses to determine which ",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("move to make. ",COLOUR::kCyan) << colour_text("MENACE",COLOUR::kRed) 
      << colour_text(" stores boards it had already evaluated in these",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("tables to prevent evaluating the same board twice. If they are reset,",COLOUR::kCyan)
      << "\n" << blank(init_gap)
      << colour_text("MENACE",COLOUR::kRed) << colour_text(" will have to evaluate all boards again.",COLOUR::kCyan)
      << "\n";
    break;
  }
  }
  std::cout.flush();
}

void UI::print_error(const ERR error, const std::vector<std::string> &args)
{
  //
  // Print the given error message with arguments
  //
  // parameters
  // ----------
  // error : const ERROR
  //  - the type of error
  // args : const std::vector<std::string> &
  //  - the print arguments
  //
  // throws
  // ------
  // invalid_print_args
  //  - if the incorrect number of arguments has been given2
  //

  const size_t size=args.size();
  switch (error)
  {
  case ERR::kUnkownCommand:
  {
    if (size!=1) { throw(invalid_print_args(1,size)); }
    std::cout << blank(3) << colour_text(args[0],COLOUR::kPurple)
      << colour_text(" is not a known command.",COLOUR::kRed)
      << "\n" << blank(3)
      << colour_text("Please type ",COLOUR::kRed) << colour_text("help",COLOUR::kPurple)
      << colour_text(" for help and more information.",COLOUR::kRed)
      << "\n";
    break;
  }
  case ERR::kUnknownPosition:
  {
    if (size!=1) { throw(invalid_print_args(1,size)); }
    std::cout << blank(3) << colour_text(args[0],COLOUR::kPurple) << colour_text(" is not a valid position.",COLOUR::kRed) 
      << "\n";
    break;
  }
  case ERR::kInvalidPosition:
  {
    if (size!=2) { throw(invalid_print_args(2,size)); }
    std::cout << blank(3)  << colour_text("The ",COLOUR::kRed) << colour_text(args[0],COLOUR::kPurple)
      << colour_text(" is already taken by ",COLOUR::kRed) << colour_text(args[1],COLOUR::kPurple)
      << colour_text(".",COLOUR::kRed) << "\n";
    break;
  }
  case ERR::kInvalidArguments:
  {
    if (size<2) { throw(invalid_print_args(2,size)); }
    std::cout << blank(3);
    for (std::vector<std::string>::const_iterator itr=std::next(std::cbegin(args)),end=std::cend(args);itr!=end;++itr)
    {
      if (std::next(itr)==end&&size>2) { std::cout << colour_text("and ",COLOUR::kRed) ; }
      std::cout << colour_text(*itr,COLOUR::kPurple);
      if (std::next(itr)!=end) { std::cout << colour_text(", ",COLOUR::kRed) ; }
    }
    std::string verb="are",plural="s",article="\b";
    if (size==2)
    {
      verb="is";
      plural="";
      article="a";
    }
    std::cout << colour_text(" "+verb+" not "+article+" valid argument"+plural+" to ",COLOUR::kRed) 
      << colour_text(args[0],COLOUR::kPurple) << colour_text(".",COLOUR::kRed) << "\n";
    break;
  }
  case ERR::kInvalidArgumentNumber:
  {
    if (size!=2) { throw(invalid_print_args(2,size)); }
    std::string number=args[1];
    COLOUR colour=COLOUR::kPurple;
    if (number=="1")
    {
      number="any";
      colour=COLOUR::kRed;
    }
    std::cout << blank(3) << colour_text(args[0],COLOUR::kPurple) << colour_text(" does not take ",COLOUR::kRed) 
      << colour_text(number,colour) << colour_text(" arguments.",COLOUR::kRed) << "\n";
    break;
  }
  case ERR::kInvalidHelpArgument:
  {
    if (size!=1) { throw(invalid_print_args(1,size)); }
    std::cout << blank(3) << colour_text("There is no additional help for ",COLOUR::kRed) << colour_text(args[0],COLOUR::kPurple)
      << colour_text(".",COLOUR::kRed) << "\n";
    break;
  }
  case ERR::kError:
  {
    std::cout << blank(3) << colour_text("An unkown or unexpected error has occured.",COLOUR::kRed) << "\n";
    break;
  }
  }
  std::cout.flush();
}

void UI::print_exit(void)
{
  //
  // Print the exit message
  //

  std::cout << "\n" << blank(3) << colour_text("Thank you for playing MENACE!",COLOUR::kCyan) << "\n";
  std::cout.flush();
}

void UI::print_result(const RESULT result, const GAME game)
{
  //
  // Print the result of a game
  //
  // parameters
  // ----------
  // result : const RESULT
  //  - the result of the game
  // result : const GAME
  //  - the type of game
  //

  if (game==GAME::kPvP)
  {
    if (result==RESULT::kDraw)
    {
      std::cout << blank(3) << colour_text("It's a draw!",COLOUR::kRed) << "\n";
    }
    else
    {
      std::string winner=result==RESULT::kCross ? "One" : "Two";
      std::cout << blank(3) << colour_text("Player "+winner+" wins!",COLOUR::kGreen) << "\n";
    }
  }
  else if (game==GAME::kMvM)
  {
    if (result==RESULT::kDraw)
    {
      std::cout << blank(3) << colour_text("It's a draw!",COLOUR::kRed) << "\n";
    }
    else
    {
      std::string winner=result==RESULT::kCross ? "One" : "Two", loser=result==RESULT::kCross ? "Two" : "One";
      std::cout << blank(3) << colour_text("MEANCE "+winner+" has beaten MENACE "+loser+".",COLOUR::kRed) << "\n"
        << blank(3) << colour_text("This really should not have happened...",COLOUR::kRed) << "\n";
    }
  }
  else if (game==GAME::kRvR)
  {
    if (result==RESULT::kDraw)
    {
      std::cout << blank(3) << colour_text("It's a draw!",COLOUR::kRed) << "\n";
    }
    else
    {
      std::string winner=result==RESULT::kCross ? "One" : "Two", loser=result==RESULT::kCross ? "Two" : "One";
      std::cout << blank(3) << colour_text("Random "+winner+" has beaten Random "+loser+"!",COLOUR::kGreen) << "\n";
    }
  }
  else if (game==GAME::kMvP&&result==RESULT::kCross||game==GAME::kPvM&&result==RESULT::kNought)
  {
    std::cout << blank(3) << colour_text("You have been beaten by MENACE!",COLOUR::kRed) << "\n";
  }
  else if ((game==GAME::kMvR||game==GAME::kRvM)&&result==RESULT::kDraw)
  {
    std::cout << blank(3) << colour_text("MENACE and Random have drawn!",COLOUR::kRed) << "\n";
  }
  else if (game==GAME::kMvR&&result==RESULT::kCross||game==GAME::kRvM&&result==RESULT::kNought)
  {
    std::cout << blank(3) << colour_text("MENACE has beaten Random!",COLOUR::kRed) << "\n";
  }
  else if (game==GAME::kMvR&&result==RESULT::kNought||game==GAME::kRvM&&result==RESULT::kCross)
  {
    std::cout << blank(3) << colour_text("Random has beaten MENACE!",COLOUR::kRed) << "\n"
      << blank(3) << colour_text("This really should not have happened...",COLOUR::kRed) << "\n";
  }
  else if (game==GAME::kMvP&&result==RESULT::kNought||game==GAME::kPvM&&result==RESULT::kCross)
  {
    std::cout << blank(3) << colour_text("You have beaten MENACE!",COLOUR::kRed) << "\n"
      << blank(3) << colour_text("This really should not have happened...",COLOUR::kRed) << "\n";
  }
  std::cout << "\n";
  std::cout.flush();
}

void UI::print_board(void)
{
  //
  // Print the board
  //

  std::cout << *display_board << "\n";
  std::cout.flush();
}

void UI::move(const short x, const short y, const WINDOWS::COORD &origin)
{
  //
  // move a certain offset from the UI origin
  //
  // parameters
  // ----------
  // x,y : const short
  //  - the horizontal and vertical offset from the origin
  // origin : const WINDOWS::COORD &
  //  - the UI origin
  //

  WINDOWS::COORD c={origin.X+x,origin.Y+y};
  SetConsoleCursorPosition(std_handle,c);
}

[[nodiscard]] WINDOWS::COORD UI::origin(void)
{
  //
  // Returns current cursor position
  //
  // returns
  // -------
  // WINDOWS::COORD
  //  - x,y coordinates of current cursor position
  //

  WINDOWS::CONSOLE_SCREEN_BUFFER_INFO info;
  WINDOWS::GetConsoleScreenBufferInfo(std_handle, &info);
  return info.dwCursorPosition;
}

void UI::clear(const short x, const short y, const int n, const WINDOWS::COORD &origin)
{
  //
  // Clear a given number of spaces from a certain offset from the UI origin
  //
  // parameters
  // ----------
  // x,y : const short
  //  - the horizontal and vertical offset from the origin
  // n : const int
  //  - the number of spaces to clear
  // origin : const WINDOWS::COORD &
  //  - the UI origin
  //

  this->move(x,y,origin);
  this->clear(n);
}

void UI::clear(const int n)
{
  //
  // Clear a given number of spaces from the current position
  //
  // parameters
  // ----------
  // m : const int
  //  - the number of spaces
  //

  std::cout << this->blank(n);
  std::cout.flush();
}

void UI::clear(const CLEAR type, const WINDOWS::COORD &origin)
{
  //
  // Perform a specific clearing procedure
  //
  // parameters
  // ----------
  // type : const CLEAR
  //  - the clearing procedure
  // origin : const WINDOWS::COORD &
  //  - the UI origin
  //

  switch (type)
  {
  case CLEAR::kPlayHelp:
  {
    for (short itr=9;itr<34;++itr)
    {
      this->clear(0,itr,77,origin);
    }
    return;
  }
  }
}

void UI::clear(void)
{
  //
  // Overload of clear to print the topic screen
  //

  this->topic_screen();
}

std::string UI::blank(const int n)
{
  //
  // Produce a blank string of a specfied number of spaces
  //
  // parameters
  // n : const int
  //  - the number of spaces
  //
  // returns
  // -------
  // std::string
  //  - a string of n spaces
  //

  return std::string(n,' ');
}

std::string UI::colour_text(const std::string &str, const COLOUR col)
{
  //
  // Colour text a given colour
  //
  // parameters
  // ----------
  // str : const std::string &
  //  - the text to colour
  // col : const COLOUR
  //  - the colour to make the text
  //
  // returns
  // -------
  // std::string
  //  - the string with the appropriate shell colouring
  //
  // throws
  // ------
  // unattributed_colour
  //  - if the given colour has no shell colouring string
  //

  if (kColours.find(col)==std::cend(kColours)) { throw unattributed_colour(col); }

  return kColours.at(col)+str+kColourEnd;
}

void UI::set_advisial(const POSITION pos)
{
  //
  // Mark a given position as an advisial move not an actual move
  //
  // parameters
  // ----------
  // pos : const POSITION
  //  - the position to mark
  //

  UI::advisial[static_cast<int>(pos)]=true;
}

void UI::reset_advisial(void)
{
  //
  // Reset UI::advisial to all false
  //

  std::fill(std::begin(UI::advisial),std::end(UI::advisial),false);
}

std::ostream &operator<<(std::ostream &os, const Board &board)
{
  //
  // std::ostream operator<<
  // For output with std::cout
  //
  // parameters
  // ----------
  // os : std::ostream &
  //  -
  // board : const Board &
  //  -
  //
  // returns
  // -------
  // std::ostream &
  //  - the ostream with the board written to it.
  //
  
  for (const BoardElement &itr:board)
  {
    if (itr.pos%3==0) // start of row
    {
      os << "\n ";
    }

    // determine who fills the position
    const VALUE val=itr.val;
    std::string disp=kValueStrings.at(val);
    COLOUR col=kValueColours.at(val);

    if (UI::advisial[itr.ind]) { col=COLOUR::kGreen; }

    os << UI::colour_text(disp,col);
    if (itr.pos%3!=2) // end of column
    { 
      os << " | ";
    }
    else if (itr.pos/3!=2) // end row but not end of board
    {
      os << "\n " << std::setfill('-') << std::setw(9) << "" << std::setfill(' ');
    }
  }
  os.flush();
  return os;
}
} // namespace menace
