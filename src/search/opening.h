#pragma once

#include "../move/move.h"
#include "../game.h"
#include <vector>

std::vector<Move::Encoded> searchOpening(const GameState &state);
