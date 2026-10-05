#include "board/fen.h"
#include "board/print.h"
#include "move/generate.h"
#include "move/move.h"
#include "types.h"
#include <iostream>

int main() {
  GameState state = loadFEN();
  Move::List movesList = generateMoves(state);
}
