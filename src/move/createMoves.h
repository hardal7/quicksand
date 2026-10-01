#pragma once

#include "../../include/types.h"
#include <optional>

using move = uint16_t;
move createMove(
    int OriginSquare, int destinationSquare,
    std::optional<Board::Piece> promotionPiece = Board::Indexes::None,
    std::optional<bool> isCastle = false);
