#include "board/board.h"
#include "board/fen.h"
#include "board/print.h"
#include "interface/uci.h"
#include "move/create.h"
#include "move/encode.h"
#include "search/search.h"
#include <iostream>

void gameLoop() {
  GameState state;

  std::string lastCmd = "position startpos moves";
  while (true) {
    command c = handleUCI(lastCmd);

    if (c.cmd == Commands::Quit) {
      break;
    }

    if (c.cmd == Commands::NewGame) {
      loadFEN(state);
    }

    if (c.cmd == Commands::UserMove) {
      if (c.val != NO_MOVE) {
        lastCmd += " " + c.val;
        move m = encodeMove(state, c.val);
        makeMove(state, m);
        printBoard(state);
      }

      std::cerr << "Thinking best move..." << std::endl;
      const int DEPTH = 4;
      int nodes = 0;
      move bestMove = searchBestMove(state, DEPTH, DEPTH, nodes);

      const int MILLION = 1'000'000;
      std::cerr << "Searched " << nodes / MILLION << "M nodes" << std::endl;

      makeMove(state, bestMove);
      printBoard(state);

      lastCmd += " " + decodeMove(bestMove);
      handleUCI(lastCmd, command{BestMove, decodeMove(bestMove)});
    }
  }
}

int main() {
  gameLoop();
  return 0;
}
