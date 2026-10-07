#pragma once

#include "../types.h"
#include <cstdint>

using Key = uint64_t;

Key GetZobristKey(const GameState &state);
