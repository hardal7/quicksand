#pragma once

#include "../board/board.h"

const int INFINITY = 100000;
int searchBestMove(GameState &state, int depth, int rootDepth, int &nodes,
                   int alpha = -INFINITY, int beta = INFINITY);
