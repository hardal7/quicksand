#pragma once

#include "board/board.h"
#include <cstdint>
#include <map>

struct GameState : Board {
  bool WhiteToPlay = true;

  bool WhiteShortCastle = true;
  bool WhiteLongCastle = true;
  bool BlackShortCastle = true;
  bool BlackLongCastle = true;

  int enPassantSquare = 0;
};
