#include "uci.h"
#include "../board/fen.h"
#include "../board/print.h"
#include "../move/move.h"
#include "../search/search.h"
#include "../types.h"
#include <iostream>
#include <sstream>
#include <string>

int countWords(std::string str) {
  std::stringstream ss(str);
  int count = 0;

  std::string word;
  while (ss >> word) {
    count++;
  }

  return count;
}

void handleUCI(GameState &state, int &moveTimeSeconds) {
  std::string cmd;
  std::getline(std::cin, cmd);

  if (cmd == "isready") {
    std::cout << "readyok" << std::endl;
    return;
  }

  else if (cmd == "uci") {
    const std::string ENGINE_NAME = "quicksand", AUTHOR_NAME = "hardal";
    std::cout << "id name " << ENGINE_NAME << std::endl;
    std::cout << "id author " << AUTHOR_NAME << std::endl;
    std::cout << "uciok" << std::endl;
    return;
  }

  else if (cmd == "quit") {
    std::exit(0);
  }

  else if (cmd.find("go") != std::string::npos) {
    std::istringstream in(cmd);
    std::string whiteTime = "0", blackTime = "0", whiteIncr = "0", blackIncr = "0";

    std::string token = "";
    while (in >> token) {
      if (token == "wtime")
        in >> whiteTime;
      else if (token == "btime")
        in >> blackTime;
      else if (token == "winc")
        in >> whiteIncr;
      else if (token == "binc")
        in >> blackIncr;
    }

    const int MIN_TIME_SECONDS = 1;
    const int MILLISECOND = 1'000;

    std::string colorTime = state.WhiteToPlay ? whiteTime : blackTime;
    std::string colorIncr = state.WhiteToPlay ? whiteIncr : blackIncr;
    moveTimeSeconds = (std::stoi(whiteTime) / 20 + std::stoi(whiteIncr) / 2) / MILLISECOND;
    moveTimeSeconds = std::max(moveTimeSeconds, MIN_TIME_SECONDS);

    Move::Encoded bestMove = searchBestMove(state, moveTimeSeconds);
    Move::make(state, bestMove);
    printBoard(state);
    std::cerr << "Best move: " << Move::annotation(bestMove) << std::endl;
    std::cout << "bestmove " << Move::annotation(bestMove) << std::endl;

    return;
  }

  else if (cmd.find("position") != std::string::npos) {
    if (cmd.find("fen") != std::string::npos) {
      std::string fen = cmd;
      size_t pos = fen.find("position fen ");
      fen.erase(0, pos + std::string("position fen ").length());
      pos = fen.find(" moves ");
      if (pos != std::string::npos) {
        fen.erase(pos);
      }

      state = loadFEN(fen);
    } else {
      state = loadFEN();
    }

    if (cmd.find("moves") != std::string::npos) {
      size_t pos = cmd.find(" moves ");
      cmd.erase(0, pos + std::string(" moves ").length());

      std::istringstream in(cmd);
      std::string word;
      while (in >> word) {
        Move::make(state, Move::encode(state, word));
      }
    }
    printBoard(state);
  }
}
