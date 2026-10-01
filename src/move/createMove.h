#pragma once

#include "../../include/enums.h"
#include "../../include/types.h"
#include <optional>

using move = uint16_t;
void createMove(
    int OriginSquare, int destinationSquare,
    std::array<move, MAX_MOVES> &movesList,
    std::optional<Board::Piece> promotionPiece = Board::Indexes::None,
    std::optional<bool> isCastle = false);

Board::Piece makeMove(GameState &state, move m);
