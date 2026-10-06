#include "board/fen.h"
#include "board/print.h"
#include "interface/uci.h"
#include "move/move.h"
#include "search/search.h"
#include "types.h"
#include <iostream>

int main() {
  GameState state;
  int moveTimeSeconds = 5;

  while (true) {
    Request req = handleUCI();

    if (req.Command == NewGame) {
      state = loadFEN();
    }

    else if (req.Command == StartGame) {
      printBoard(state);
      Move::Encoded bestMove = searchBestMove(state, moveTimeSeconds);

      handleUCI(Request{MakeMove, Move::annotation(bestMove)});
      Move::make(state, bestMove);
      printBoard(state);
      std::cerr << "Best Move: " << Move::annotation(bestMove) << std::endl;
    }

    else if (req.Command == MakeMove) {
      Request timereq = handleUCI();
      if (timereq.Command == SetTime) {
        SetTimeVal setTime = std::get<SetTimeVal>(timereq.Value);
        moveTimeSeconds = state.WhiteToPlay ? setTime[0] : setTime[1];
        std::cerr << "Set time to: " << moveTimeSeconds << std::endl;
      }

      Move::make(state, Move::encode(state, std::get<std::string>(req.Value)));
      printBoard(state);
      std::cerr << "Made Move: " << std::get<std::string>(req.Value) << std::endl;

      Move::Encoded bestMove = searchBestMove(state, moveTimeSeconds);
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
