#pragma once

#include "../types.h"
#include <string>

const std::string startingString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
void loadFEN(GameState &state, std::string fen = startingString);
