#include "../include/types.h"
#include "move/createMove.h"
#include "move/generateMoves.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"

int main() {
  GameState state;
  loadFEN(state);
  printBoard(state);
  std::array<move, MAX_MOVES> moves = {0};
  generateMoves(state, moves);
  makeMove(state, moves[0]);
  printBoard(state);

  return 0;
}
