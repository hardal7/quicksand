#include "../../include/types.h"
#include "../eval/eval.h"
#include "../move/create.h"
#include "../move/encode.h"
#include "../move/generate.h"

const int INFINITY = 100000;

int searchBestMove(GameState &state, int depth, int rootDepth, int &nodes) {
  int bestEval = INFINITY * (state.whiteToPlay ? -1 : 1);

  std::array<move, MAX_MOVES> moves = {0};
  generateMoves(state, moves);
  move bestMove = 0;
  for (move m : moves) {
    if (m == 0) {
      break;
    }

    Board::Piece capturedPiece = makeMove(state, m);
    nodes++;

    if (depth != 1) {
      searchBestMove(state, depth - 1, rootDepth, nodes);
    }

    bool movedColor = !state.whiteToPlay;
    int eval = evaluateBoard(state);
    if (movedColor ? (eval > bestEval) : (eval < bestEval)) {
      bestEval = eval;
      if (depth == rootDepth) {
        bestMove = m;
      }
    }

    unmakeMove(state, m, capturedPiece);
  }

  return depth == rootDepth ? bestMove : bestEval;
}
