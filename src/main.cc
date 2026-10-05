#include "board/fen.h"
#include "board/print.h"
#include "interface/uci.h"
#include "move/move.h"
#include "search/search.h"
#include "types.h"
#include <iostream>

const int depth = 5;
int main() {
  GameState state;

  while (true) {
    Request req = handleUCI();

    if (req.Command == NewGame) {
      state = loadFEN();
    }

    else if (req.Command == StartGame) {
      printBoard(state);
      Move::Encoded bestMove = searchBestMove(state, depth);

      handleUCI(Request{MakeMove, Move::annotation(bestMove)});
      Move::make(state, bestMove);
      printBoard(state);
      std::cerr << "Best Move: " << Move::annotation(bestMove) << std::endl;
    }

    else if (req.Command == MakeMove) {
      Move::make(state, Move::encode(state, req.Move));
      printBoard(state);
      std::cerr << "Made Move: " << req.Move << std::endl;

      Move::Encoded bestMove = searchBestMove(state, depth);
      handleUCI(Request{MakeMove, Move::annotation(bestMove)});
      Move::make(state, bestMove);
      printBoard(state);
      std::cerr << "Best Move: " << Move::annotation(bestMove) << std::endl;
    }

    else if (req.Command == Quit) {
      break;
    }
  }

  return 0;
}
