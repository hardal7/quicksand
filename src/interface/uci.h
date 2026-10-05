#include <string>

enum Cmd {
  None,
  NewGame,
  StartGame,
  MakeMove,
  Quit,
};

const std::string NoMove = "::NO_MOVE::";

struct Request {
  Cmd Command = None;
  std::string Move;
};

Request handleUCI(Request req = Request{None, NoMove});
