#pragma once

#include "../../include/types.h"
#include <string>

const std::string startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
void loadFEN(GameState &state, std::string fen = startingFEN);
