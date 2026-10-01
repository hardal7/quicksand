#pragma once

#include "../../include/enums.h"
#include "../../include/types.h"
#include "encode.h"
#include <optional>

void createMove(int OriginSquare, int destinationSquare,
                std::array<move, MAX_MOVES> &movesList,
                std::optional<Board::Piece> promotionPiece = Board::None,
                std::optional<bool> isCastle = false);

Board::Piece makeMove(GameState &state, move m);
void unmakeMove(GameState &state, move m, Board::Piece capturedPiece);
