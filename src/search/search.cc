#include "../eval/eval.h"
#include "../move/move.h"
#include "../types.h"
#include <chrono>
#include <climits>
#include <iostream>

const int INFINITY = 1'000'000;
const int TIMEOUT = INT_MAX;

int searchPosition(GameState &state, int depth, int &nodes, const std::chrono::steady_clock::time_point &deadline,
                   int alpha = -INFINITY, int beta = INFINITY) {
  if (std::chrono::steady_clock::now() >= deadline) {
    return TIMEOUT;
  }
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

    eval = searchPosition(state, depth - 1, nodes, deadline, alpha, beta);

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

Move::Encoded searchBestMove(GameState &state, int timeSeconds) {
  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(timeSeconds);

  bool maximizing = state.WhiteToPlay;

  Move::Encoded bestMove = Move::NoMove, bestMoveCandidate = Move::NoMove;
  Move::List moves = Move::generate(state);

  int nodes = 0;
  for (int depth = 1; depth < INFINITY; depth++) {
    int bestEval = maximizing ? -INFINITY : INFINITY;
    int eval = bestEval;
    int alpha = -INFINITY, beta = INFINITY;

    for (int i = 0; i < moves.length; i++) {
      Move::Encoded m = moves.list[i];
      auto captured = Move::make(state, m);

      eval = searchPosition(state, depth - 1, nodes, deadline, alpha, beta);

      Move::unmake(state, m, captured);

      if (maximizing) {
        if (eval > bestEval) {
          bestMoveCandidate = m;
          bestEval = eval;
        }
        alpha = std::max(alpha, bestEval);
      } else {
        if (eval < bestEval) {
          bestMoveCandidate = m;
          bestEval = eval;
        }
        beta = std::min(beta, bestEval);
      }

      if (beta <= alpha) {
        break;
      }
    }

    if (std::chrono::steady_clock::now() < deadline) {
      bestMove = bestMoveCandidate;
    } else {
      break;
    }
  }

  std::cerr << std::endl << "Searched: " << nodes << " nodes" << std::endl;
  return bestMove;
}
