#include "../include/types.h"
#include "eval/eval.h"
#include "move/create.h"
#include "move/encode.h"
#include "move/generate.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"
#include <iostream>

move searchBestMove(GameState &state, int depth, int &nodes) {
  move bestMove = 0;
  std::array<move, MAX_MOVES> moves = {0};
  generateMoves(state, moves);
  for (move m : moves) {
    if (m == 0) {
      break;
    }
    bestMove = m;

    std::cout << decodeMove(m) << std::endl;
    Board::Piece capturedPiece = makeMove(state, m);
    nodes++;

    if (depth != 1) {
      bestMove = searchBestMove(state, depth - 1, nodes);
    }

    unmakeMove(state, m, capturedPiece);
  }
  return bestMove;
}

void gameLoop(GameState &state) {
  while (true) {
    printBoard(state);
    std::cout << (state.whiteToPlay ? "White" : "Black") << " to play"
              << std::endl;

    if (state.whiteToPlay) {
      std::string userMove = "";
      std::cout << "Enter move: ";
      std::cin >> userMove;

      if (userMove == "q" || userMove == "quit") {
        return;
      }

      makeMove(state, encodeMove(state, userMove));
    } else {
      const int DEPTH = 1;
      int nodes = 0;
      move bestMove = searchBestMove(state, DEPTH, nodes);
      makeMove(state, bestMove);
      std::cout << "Made move: " << decodeMove(bestMove) << std::endl;
      std::cout << "Searched " << nodes << " nodes" << std::endl;
    }
  }
}

int main() {
  GameState state;
  loadFEN(state);
  printBoard(state);
  std::cout << "Evaluation: " << evaluateBoard(state) << std::endl;
  // loadFEN(state, "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
  // gameLoop(state);

  return 0;
}
