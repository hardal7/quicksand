#pragma once

#include "../game.h"
#include <cstdint>

using Key = uint64_t;

Key getZobristKey(const GameState &state);
