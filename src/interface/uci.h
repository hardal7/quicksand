#include "../board/board.h"
#include <array>
#include <string>
#include <variant>

enum Cmd {
  None,
  NewGame,
  StartGame,
  MakeMove,
  SetTime,
  Quit,
};

const std::string NoMove = "::NO_MOVE::";

using SetTimeVal = std::array<int, Board::TotalColors>;

struct Request {
  Cmd Command = None;
  std::variant<SetTimeVal, std::string> Value;
};

Request handleUCI(Request req = Request{None, NoMove});
