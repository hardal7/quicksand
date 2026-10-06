#include "uci.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

const std::string ENGINE_NAME = "quicksand", AUTHOR_NAME = "hardal";
const int MIN_TIME_SECONDS = 1;
const int MILLISECOND = 1'000;

Request handleUCI(Request req) {
  if (req.Command == MakeMove) {
    std::cout << "bestmove " << std::get<std::string>(req.Value) << std::endl;
    return Request{None, NoMove};
  }

  std::string cmd = "";
  std::getline(std::cin, cmd);

  if (cmd == "uci") {
    std::cout << "id name " << ENGINE_NAME << std::endl;
    std::cout << "id author " << AUTHOR_NAME << std::endl;
    std::cout << "uciok" << std::endl;
  }

  else if (cmd == "isready") {
    std::cout << "readyok" << std::endl;
    return Request{NewGame, NoMove};
  }

  else if (cmd.find("position startpos") != std::string::npos) {
    std::string move = "";
    if (cmd.find("position startpos moves") != std::string::npos) {
      size_t pos = cmd.find_last_of(' ');
      move = cmd.substr(pos + 1);
      return Request{MakeMove, move};
    } else {
      return Request{StartGame, NoMove};
    }
  }

  else if (cmd.find("go") != std::string::npos) {
    std::istringstream in(cmd);
    std::string whiteTime = "0", blackTime = "0", whiteIncr = "0", blackIncr = "0";

    std::string token = "";
    while (in >> token) {
      if (token == "wtime")
        in >> whiteTime;
      else if (token == "btime")
        in >> blackTime;
      else if (token == "winc")
        in >> whiteIncr;
      else if (token == "binc")
        in >> blackIncr;
    }

    int whiteMoveTime = (std::stoi(whiteTime) / 20 + std::stoi(whiteIncr) / 2) / MILLISECOND;
    int blackMoveTime = (std::stoi(blackTime) / 20 + std::stoi(blackIncr) / 2) / MILLISECOND;

    whiteMoveTime = std::max(whiteMoveTime, MIN_TIME_SECONDS);
    blackMoveTime = std::max(blackMoveTime, MIN_TIME_SECONDS);

    return Request{SetTime, SetTimeVal{whiteMoveTime, blackMoveTime}};
  }

  else if (cmd == "quit") {
    std::cout << "Quit requested, quitting." << std::endl;
    return Request{Quit, NoMove};
  }

  return Request{None, NoMove};
}
