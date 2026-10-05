#include "../eval/eval.h"
#include "../move/move.h"
#include "../types.h"
#include <iostream>

const int INFINITY = 1'000'000;

int searchPosition(GameState &state, int depth, int &nodes, int alpha = -INFINITY, int beta = INFINITY) {
  if (depth == 0) {
    nodes++;
    return evaluatePosition(state);
  }

  bool maximizing = state.WhiteToPlay;
  int bestEval = maximizing ? -INFINITY : INFINITY;
  int eval = bestEval;

  Move::List moves = Move::generate(state);
  for (int i = 0; i < moves.length; i++) {
    Move::Encoded m = moves.list[i];
    auto captured = Move::make(state, m);

    eval = searchPosition(state, depth - 1, nodes, alpha, beta);

    Move::unmake(state, m, captured);

    if (maximizing) {
      bestEval = std::max(bestEval, eval);
      alpha = std::max(alpha, bestEval);
    } else {
      bestEval = std::min(bestEval, eval);
      beta = std::min(beta, bestEval);
    }

    if (beta <= alpha) {
      break;
    }
  }

  return bestEval;
}

Move::Encoded searchBestMove(GameState &state, int depth) {
  bool maximizing = state.WhiteToPlay;
  int bestEval = maximizing ? -INFINITY : INFINITY;
  int eval = bestEval;

  Move::Encoded bestMove = Move::NoMove;
  Move::List moves = Move::generate(state);

  int nodes = 0;
  int alpha = -INFINITY, beta = INFINITY;
  for (int i = 0; i < moves.length; i++) {
    Move::Encoded m = moves.list[i];
    auto captured = Move::make(state, m);

    eval = searchPosition(state, depth - 1, nodes, alpha, beta);

    Move::unmake(state, m, captured);

    if (maximizing) {
      if (eval > bestEval) {
        bestMove = m;
        bestEval = eval;
      }
      alpha = std::max(alpha, bestEval);
    } else {
      if (eval < bestEval) {
        bestMove = m;
        bestEval = eval;
      }
      beta = std::min(beta, bestEval);
    }

    if (beta <= alpha) {
      break;
    }
  }

  std::cerr << "Searched: " << nodes << " nodes" << std::endl;
  return bestMove;
}
