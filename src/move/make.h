#pragma once

#include "../board/board.h"
#include "encode.h"
#include "enums.h"

enum SpecialMove { None = 0, Promotion = 1, Castle = 2, EnPassant = 3 };
struct moveType {
  SpecialMove type = None;
  Board::Piece promotionPiece = Board::None;
};

void createMove(int originSquare, int destinationSquare,
                std::array<move, MAX_MOVES> &movesList,
                moveType type = moveType{});

Board::Piece makeMove(GameState &state, move m);
void unmakeMove(GameState &state, move m, Board::Piece capturedPiece);
