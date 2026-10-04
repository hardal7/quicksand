#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

namespace Move {
// clang-format off
   const uint16_t OriginSquareMask =      0b0000000000111111;
   const uint16_t DestinationSquareMask = 0b0000111111000000;
   const uint16_t FlagMask =              0b0011000000000000;
   const uint16_t PromotionPieceMask =    0b1100000000000000;
// clang-format on
const int OriginSquareOffset = 0;
const int DestinationSquareOffset = 6;
const int FlagOffset = 12;
const int PromotionPieceOffset = 14;

using Encoded = uint16_t;

enum Flags { None, Promotion, Castle, EnPassant };
const int PROMOTION_PIECES = 4;
enum PromotionPieces { Knight, Bishop, Rook, Queen };

struct Readable {
  int OriginSquare;
  int DestinationSquare;
  Flags Flag;
  PromotionPieces PromotionPiece;
};

Encoded encode(Readable move);

const int NoMove = 0;
const int MaxMoves = 256;
using List = std::array<Encoded, MaxMoves>;

void insert(List &list, Readable m);
}; // namespace Move
