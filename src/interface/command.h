#pragma once

#include <string>

enum Commands {
  None = 0,
  Quit = 1,
  NewGame = 2,
  BestMove = 3,
  UserMove = 4,
};

const std::string NO_MOVE = "NOMOVE";

struct command {
  Commands cmd;
  std::string val;
};
