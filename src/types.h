#pragma once

#include "board/board.h"
#include <cstdint>
#include <map>

struct GameState {
  Board::bitboard Bitboards[Board::Pieces + Board::Colors] = {0};
  bool WhiteToPlay = true;

  bool WhiteShortCastle = true;
  bool WhiteLongCastle = true;
  bool BlackShortCastle = true;
  bool BlackLongCastle = true;

  int enPassantSquare = 0;
};
