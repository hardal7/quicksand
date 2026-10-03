#include "../board/board.h"
#include "../eval/eval.h"
#include "../move/encode.h"
#include "../move/generate.h"
#include "../move/make.h"

const int INFINITY = 100000;

int searchBestMove(GameState &state, int depth, int rootDepth, int &nodes) {
  int bestEval = INFINITY * (state.whiteToPlay ? -1 : 1);

  std::array<move, MAX_MOVES> moves = {NO_MOVE};
  generateMoves(state, moves);
  move bestMove = NO_MOVE;
  for (move m : moves) {
    if (m == NO_MOVE) {
      break;
    }

    Board::Piece capturedPiece = makeMove(state, m);
    nodes++;

    int eval = bestEval;
    if (depth != 1) {
      eval = searchBestMove(state, depth - 1, rootDepth, nodes);
    } else {
      eval = evaluateBoard(state);
    }

    bool movedColor = !state.whiteToPlay;
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
