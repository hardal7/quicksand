#pragma once

#include "../types.h"
#include <string>

const std::string startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
GameState loadFEN(std::string fen = startingFEN);
