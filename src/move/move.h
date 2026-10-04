#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

class Move {
  // clang-format off
  static const uint16_t OriginSquareMask =      0b0000000000111111;
  static const uint16_t DestinationSquareMask = 0b0000111111000000;
  static const uint16_t FlagMask =              0b0011000000000000;
  static const uint16_t PromotionPieceMask =    0b1100000000000000;
  // clang-format on
  static const int OriginSquareOffset = 0;
  static const int DestinationSquareOffset = 6;
  static const int FlagOffset = 12;
  static const int PromotionPieceOffset = 14;

public:
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

  static Encoded encode(Readable move) {
    Encoded encoded = NoMove;

    encoded |= ((move.OriginSquare << OriginSquareOffset) & OriginSquareMask);
    encoded |= ((move.DestinationSquare << DestinationSquareOffset) & DestinationSquareMask);
    encoded |= ((move.Flag << FlagOffset) & FlagMask);
    encoded |= ((move.PromotionPiece << PromotionPieceOffset) & PromotionPieceMask);

    return encoded;
  }

  static const int NoMove = 0;
  static const int MaxMoves = 256;
  using List = std::array<Encoded, MaxMoves>;

  static void insert(List &list, Readable m) {
    auto it = std::find(list.begin(), list.end(), NoMove);
    int index = it - list.begin();

    list[index] = encode(m);
  }
};
