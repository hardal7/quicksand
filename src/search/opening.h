#pragma once

#include "../move/move.h"
#include "../types.h"
#include <vector>

std::vector<Move::Encoded> searchOpening(const GameState &state);
