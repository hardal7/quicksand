#pragma once

#include "../../include/enums.h"
#include "create.h"

void generateMoves(const GameState &state,
                   std::array<move, MAX_MOVES> &movesList);
