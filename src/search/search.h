#pragma once

#include "../board/board.h"
#include <atomic>

const int INFINITY = 100000;
int searchBestMove(std::atomic<bool> &timerExpired, GameState &state, int depth,
                   int rootDepth, int &nodes, int alpha = -INFINITY,
                   int beta = INFINITY);
