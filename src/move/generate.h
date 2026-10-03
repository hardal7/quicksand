#pragma once

#include "../board/board.h"
#include "encode.h"
#include "enums.h"
#include "order.h"

void generateMoves(const GameState &state,
                   std::array<ScoredMove, MAX_MOVES> &movesList);
