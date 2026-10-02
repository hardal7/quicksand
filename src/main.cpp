#include "../include/types.h"
#include "move/create.h"
#include "move/encode.h"
#include "search/search.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"
#include <iostream>

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
        std::cout << "'q' or 'quit' entered, quitting." << std::endl;
        return;
      }

      makeMove(state, encodeMove(state, userMove));
    } else {
      std::cout << "Thinking best move..." << std::endl;
      const int DEPTH = 2;
      int nodes = 0;
      move bestMove = searchBestMove(state, DEPTH, DEPTH, nodes);

      makeMove(state, bestMove);
      std::cout << "Made move: " << decodeMove(bestMove) << std::endl;

      const int MILLION = 1'000'000;
      std::cout << "Searched " << nodes / MILLION << "M nodes" << std::endl;
    }
  }
}

int main() {
  // loadFEN(state, "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
  // std::cout << "Evaluation: " << evaluateBoard(state) << std::endl;
  GameState state;
  loadFEN(state);
  gameLoop(state);
  return 0;
}
