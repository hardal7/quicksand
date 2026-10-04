#include "search.h"
#include "../board/board.h"
#include "../eval/eval.h"
#include "../move/encode.h"
#include "../move/generate.h"
#include "../move/make.h"

const int POLLING_INTERVAL = 1'000'000;

int searchBestMove(std::atomic<bool> &timerExpired, GameState &state, int depth,
                   int rootDepth, int &nodes, int alpha, int beta) {
  int bestEval = INFINITY * (state.whiteToPlay ? -1 : 1);
  if (nodes % POLLING_INTERVAL) {
    if (timerExpired) {
      return NO_MOVE;
    }
  }

  std::array<ScoredMove, MAX_MOVES> moves = {NO_MOVE};
  generateMoves(state, moves);
  orderMoves(state, moves);

  move bestMove = NO_MOVE;
  for (ScoredMove m : moves) {
    if (m.value == NO_MOVE) {
      break;
    }
    Board::Piece capturedPiece = makeMove(state, m.value);
    nodes++;

    int eval = bestEval;
    if (depth != 1) {
      eval = searchBestMove(timerExpired, state, depth - 1, rootDepth, nodes,
                            alpha, beta);
    } else {
      eval = evaluateBoard(state);
    }
    unmakeMove(state, m.value, capturedPiece);

    if (state.whiteToPlay ? (eval > bestEval) : (eval < bestEval)) {
      bestEval = eval;
      if (depth == rootDepth) {
        bestMove = m.value;
      }
    }

    if (state.whiteToPlay) {
      if (eval > alpha) {
        alpha = eval;
      }
    } else {
      if (eval < beta) {
        beta = eval;
      }
    }

    if (beta <= alpha) {
      break;
    }
  }

  return depth == rootDepth ? bestMove : bestEval;
}
