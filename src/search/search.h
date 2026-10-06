#pragma once

#include "../move/move.h"
#include "../types.h"

Move::Encoded searchBestMove(GameState &state, int timeSeconds);
