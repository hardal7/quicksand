#pragma once

#include <string>

namespace Commands {
enum Commands {
  None = 0,
  Quit = 1,
  NewGame = 2,
  BestMove = 3,
  UserMove = 4,
};
}

const std::string NO_MOVE_STR = "NOMOVE";

struct command {
  Commands::Commands cmd;
  std::string val;
};
