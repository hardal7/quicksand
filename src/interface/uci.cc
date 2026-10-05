#include "uci.h"
#include <iostream>
#include <string>

const std::string ENGINE_NAME = "quicksand", AUTHOR_NAME = "hardal";

Request handleUCI(Request req) {
  if (req.Command == MakeMove) {
    std::cout << "bestmove " << req.Move << std::endl;
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

  else if (cmd == "quit") {
    std::cout << "Quit requested, quitting." << std::endl;
    return Request{Quit, NoMove};
  }

  return Request{None, NoMove};
}
