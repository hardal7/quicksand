#include "board/board.h"
#include "board/fen.h"
#include "board/print.h"
#include "interface/uci.h"
#include "move/encode.h"
#include "move/make.h"
#include "search/search.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

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

    const int DEFAULT_MOVETIME = 5;
    int moveTimeSeconds = DEFAULT_MOVETIME;
    if (c.cmd == Commands::Time) {
      std::istringstream iss(c.val);
      std::string timeStr, incrementStr;
      iss >> timeStr >> incrementStr;

      int time = 0, increment = 0;
      time = std::stoi(timeStr);
      increment = std::stoi(incrementStr);

      moveTimeSeconds = time / 20 + increment / 2;
    }

    if (c.cmd == Commands::UserMove) {
      if (c.val != NO_MOVE_STR) {
        lastCmd += " " + c.val;
        move m = encodeMove(state, c.val);
        makeMove(state, m);
        printBoard(state);
      }

      std::cerr << "Thinking best move for "
                << (state.whiteToPlay ? "White" : "Black") << "..."
                << std::endl;

      std::atomic<bool> timerExpired = false;
      std::thread timer([moveTimeSeconds, &timerExpired] {
        std::this_thread::sleep_for(std::chrono::seconds(moveTimeSeconds));
        timerExpired = true;
      });

      const int MILLION = 1'000'000;
      int nodes = 0;
      move bestMove = NO_MOVE;
      int depth = 1;

      auto start = std::chrono::steady_clock::now();
      while (true) {
        std::cerr << "Searching at Depth: " << depth << std::endl;

        move candidateMove =
            searchBestMove(timerExpired, state, depth, depth, nodes);

        if (!timerExpired) {
          bestMove = candidateMove;
          depth++;
        } else {
          auto elapsed = std::chrono::steady_clock::now() - start;
          start = std::chrono::steady_clock::now();
          auto time =
              std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
          std::cerr << "Searched " << nodes / MILLION << "M nodes in " << time
                    << "s" << std::endl;
          break;
        }
      }
      timer.join();

      makeMove(state, bestMove);
      printBoard(state);

      lastCmd += " " + decodeMove(bestMove);
      handleUCI(lastCmd, command{Commands::BestMove, decodeMove(bestMove)});
    }
  }
}

int main() {
  gameLoop();

  return 0;
}
