#include "search.h"
#include "../eval/eval.h"
#include "../move/move.h"
#include "opening.h"
#include "order.h"
#include "transposition.h"
#include <array>
#include <chrono>
#include <climits>
#include <iostream>
#include <optional>
#include <vector>

const int INFINITY = 1'000'000;
const int TIMEOUT = INT_MAX;

int searchPosition(GameState state, TTable &ttable, int depth, int &nodes,
                   const std::chrono::steady_clock::time_point &deadline, int alpha = -INFINITY, int beta = INFINITY) {
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

  Move::List unorderedMoves = Move::generate(state);
  OrderedList moves = orderMoves(state, unorderedMoves);
  Move::Encoded bestMove = Move::NoMove;

  for (int i = 0; i < moves.length; i++) {
    Move::Encoded m = moves.list[i].Move;
    auto captured = Move::make(state, m);

    std::optional<Transposition> t = searchTranspositions(state, ttable, depth);
    if (t.has_value()) {
      eval = t->eval;
    } else {
      eval = searchPosition(state, ttable, depth - 1, nodes, deadline, alpha, beta);
    }

    Move::unmake(state, m, captured);

    if (maximizing) {
      if (eval > bestEval) {
        bestEval = eval;
        bestMove = m;
      }
      alpha = std::max(alpha, bestEval);
    } else {
      if (eval < bestEval) {
        bestEval = eval;
        bestMove = m;
      }
      beta = std::min(beta, bestEval);
    }

    if (beta <= alpha) {
      break;
    }
  }

  saveTransposition(state, ttable, depth, bestEval, bestMove);

  return bestEval;
}

Move::Encoded searchBestMove(Engine &engine) {
  std::vector<Move::Encoded> openings = searchOpening(engine.state);
  if (openings.size() != 0) {
    Move::Encoded opening = openings[rand() % openings.size()];
    std::cerr << std::endl << "Using opening move: " << Move::annotation(opening);
    return opening;
  }

  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(engine.moveTimeSeconds);

  bool maximizing = engine.state.WhiteToPlay;

  Move::Encoded bestMove = Move::NoMove, bestMoveCandidate = Move::NoMove;
  Move::List unorderedMoves = Move::generate(engine.state);
  OrderedList moves = orderMoves(engine.state, unorderedMoves);

  int nodes = 0;
  int searchedDepth = 0;
  int score = 0;

  for (int depth = 1; depth <= MAX_DEPTH; depth++) {
    int bestEval = maximizing ? -INFINITY : INFINITY;
    int eval = bestEval;
    int alpha = -INFINITY, beta = INFINITY;

    for (int i = 0; i < moves.length; i++) {
      Move::Encoded m = moves.list[i].Move;
      auto captured = Move::make(engine.state, m);

      std::optional<Transposition> t = searchTranspositions(engine.state, engine.ttable, depth);
      if (t.has_value()) {
        eval = t->eval;
      } else {
        eval = searchPosition(engine.state, engine.ttable, depth - 1, nodes, deadline, alpha, beta);
      }

      Move::unmake(engine.state, m, captured);

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
      searchedDepth = depth;
      score = bestEval / pieceValues.at(Board::Pawn);
    } else {
      break;
    }

    saveTransposition(engine.state, engine.ttable, depth, bestEval, bestMove);
  }

  std::cout << "info " << "depth " << searchedDepth << " nodes " << nodes << " time " << engine.moveTimeSeconds
            << " nps " << nodes / engine.moveTimeSeconds << " score cp " << score << std::endl;
  return bestMove;
}
