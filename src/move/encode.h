#pragma once

#include "../../include/types.h"
#include <string>

using move = uint16_t;

enum MoveEncodings {
  OriginSquareOffset = 0,
  DestinationSquareOffset = 6,
  FlagOffset = 12,
  PromotionPieceOffset = 14,

  OriginSquareOffsetMask = 0x3F,
  DestinationSquareOffsetMask = 0xFC0,
  FlagOffsetMask = 0x3000,
  PromotionPieceOffsetMask = 0xC00,

  PromotionFlag = 1,
  CastleFlag = 2,
  EnPassantFlag = 3
};

std::string decodeMove(move m);
move encodeMove(const GameState &state, std::string moveString);
