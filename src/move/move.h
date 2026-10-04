#pragma once

#include "../board/board.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <map>

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

enum Flag { NoFlag, Promotion, Castle, EnPassant };
enum PromotionPiece { Knight, Bishop, Rook, Queen };
const std::array<PromotionPiece, 4> PromotionPieces = {Knight, Bishop, Rook, Queen};

struct Readable {
  int OriginSquare;
  int DestinationSquare;
  Move::Flag Flag = NoFlag;
  Move::PromotionPiece PromotionPiece = Knight;
};

Encoded encode(Readable move);

const int NoMove = 0;
const int MaxMoves = 256;
struct List {
  std::array<Encoded, MaxMoves> list = {NoMove};
  int length = 0;
};

void insert(List &list, Readable m);

enum Direction { Up = -8, Down = 8, Left = -1, Right = 1 };

const std::map<int, Board::bitboard> Constraints = {
    {Up, Board::RankEight},
    {Down, Board::RankOne},
    {Left, Board::FileA},
    {Right, Board::FileH},

    {Up + Left, Board::RankEight | Board::FileA},
    {Up + Right, Board::RankEight | Board::FileH},
    {Down + Left, Board::RankOne | Board::FileA},
    {Down + Right, Board::RankOne | Board::FileH},

    {Up + Left * 2, Board::RankEight | Board::FileA | Board::FileB},
    {Up + Right * 2, Board::RankEight | Board::FileH | Board::FileG},
    {Down + Left * 2, Board::RankOne | Board::FileA | Board::FileB},
    {Down + Right * 2, Board::RankOne | Board::FileH | Board::FileG},

    {Up * 2 + Left, Board::RankEight | Board::RankSeven | Board::FileA},
    {Up * 2 + Right, Board::RankEight | Board::RankSeven | Board::FileH},
    {Down * 2 + Left, Board::RankOne | Board::RankTwo | Board::FileA},
    {Down * 2 + Right, Board::RankOne | Board::RankTwo | Board::FileH},
};
}; // namespace Move
