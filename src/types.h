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

  bool WhiteShortCastlePrev = true;
  bool WhiteLongCastlePrev = true;
  bool BlackShortCastlePrev = true;
  bool BlackLongCastlePrev = true;

  int enPassantSquare = 0;
  int enPassantSquarePrev = 0;
};
