#pragma once

#include "create.h"

void generateMoves(const GameState &state,
                   std::array<move, MAX_MOVES> &movesList);
