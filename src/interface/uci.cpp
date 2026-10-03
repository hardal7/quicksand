#include "command.h"
#include <iostream>
#include <string>

const std::string ENGINE_NAME = "quicksand", AUTHOR_NAME = "hardal";

command handleUCI(std::string lastCmd, command c = command{None}) {
  if (c.cmd == BestMove) {
    std::cout << "bestmove " << c.val << std::endl;
    return command{None};
  }

  std::string userCmd = "";

  std::getline(std::cin, userCmd);

  if (userCmd == "uci") {
    std::cout << "id name " << ENGINE_NAME << std::endl;
    std::cout << "id author " << AUTHOR_NAME << std::endl;
    std::cout << "uciok" << std::endl;
    return command{None};
  }

  else if (userCmd == "isready") {
    std::cout << "readyok" << std::endl;
    return command{NewGame};
  }

  else if (userCmd.find("position startpos") != std::string::npos) {
    std::string move = "";
    if (userCmd.find("position startpos moves") != std::string::npos) {
      size_t pos = userCmd.find_last_of(' ');
      move = userCmd.substr(pos + 1);
    } else {
      move = NO_MOVE;
    }
    return command{UserMove, move};
  }

  else if (userCmd == "quit") {
    return command{Quit};
  }

  return command{None};
}
