#pragma once

#include "../engine.h"
#include "../move/move.h"

const int MAX_DEPTH = 16;
Move::Encoded searchBestMove(Engine &engine);
