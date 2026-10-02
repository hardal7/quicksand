#include <iostream>
#include <string>

const std::string ENGINE_NAME = "quicksand", AUTHOR_NAME = "hardal";

std::string handleUCI(bool firstCmd) {
  std::string lastCmd = "";

  std::string cmd = "";
  std::getline(std::cin, cmd);

  if (cmd == "uci") {
    std::cout << "id name " << ENGINE_NAME << std::endl;
    std::cout << "id author " << AUTHOR_NAME << std::endl;
    std::cout << "uciok" << std::endl;
    return "";
  }

  else if (cmd == "isready") {
    std::cout << "readyok" << std::endl;
    return "";
  }

  else if (cmd.find("position startpos") != std::string::npos) {
    if (firstCmd) {
      if (cmd.find("moves") != std::string::npos) {
        lastCmd = "position startpos ";
      } else {
        lastCmd = "position startpos moves ";
      }
    }

    std::string moveString = cmd;
    int pos = moveString.find(lastCmd);
    if (pos != std::string::npos) {
      cmd.erase(pos, lastCmd.length());
    }
  }

  else if (cmd == "quit") {
    return cmd;
  }

  lastCmd = cmd;
  return cmd;
}

// bestmove e2e4
