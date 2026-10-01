#pragma once

#include "../../include/enums.h"
#include "../../include/types.h"
#include "create.h"
#include <cstdint>
#include <optional>

void generateMoves(const GameState &state,
                   std::array<move, MAX_MOVES> &movesList);
