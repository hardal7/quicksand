#pragma once

#include "../board/board.h"
#include "encode.h"
#include "enums.h"

void generateMoves(const GameState &state,
                   std::array<move, MAX_MOVES> &movesList);
