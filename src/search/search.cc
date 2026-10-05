#include "../eval/eval.h"
#include "../move/move.h"
#include "../types.h"
#include <iostream>

const int INFINITY = 1000000;

int searchPosition(GameState &state, int depth, int &nodes) {
  Move::List moves = Move::generate(state);
  int bestEval = INFINITY * (state.WhiteToPlay ? -1 : 1);
  int eval = bestEval;

  for (int i = 0; i < moves.length; i++) {
    Move::Encoded m = moves.list[i];

    bool white = state.WhiteToPlay;
    auto captured = Move::make(state, m);
    nodes++;

    if (depth > 0) {
      eval = searchPosition(state, depth - 1, nodes);
    } else {
      eval = evaluatePosition(state);
    }

    if (white ? (eval > bestEval) : (eval < bestEval)) {
      bestEval = eval;
    }

    Move::unmake(state, m, captured);
  }

  return bestEval;
}

Move::Encoded searchBestMove(GameState &state, int depth) {
  Move::List moves = Move::generate(state);
  Move::Encoded bestMove = Move::NoMove;
  int bestEval = INFINITY * (state.WhiteToPlay ? -1 : 1);
  int eval = bestEval;

  int nodes = 0;
  for (int i = 0; i < moves.length; i++) {
    Move::Encoded m = moves.list[i];

    bool white = state.WhiteToPlay;
    auto captured = Move::make(state, m);

    eval = searchPosition(state, depth - 1, nodes);
    std::cerr << "Move: " << Move::annotation(m) << " Eval: " << eval << std::endl;

    if (white ? (eval > bestEval) : (eval < bestEval)) {
      bestEval = eval;
      bestMove = m;
    }

    Move::unmake(state, m, captured);
  }

  std::cerr << "Searched: " << nodes << " nodes" << std::endl;
  return bestMove;
}
