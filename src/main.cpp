#include <cstdint>
#include <iostream>
#include <string>

#include "utils/loadFEN.hpp"
#include "utils/enums.hpp"
#include "utils/annotateMove.hpp"
#include "utils/decodeMove.hpp"
#include "utils/visualizeBoard.hpp"
#include "search/searchBestMove.hpp"
#include "eval/evaluateBoard.hpp"
#include "move/makeMove.hpp"
#include "move/moveGeneration.hpp"

uint64_t pieceBitboard[8] = {0};

int startGame() {
  std::string input = "";
  std::string lastCommand = "position startpos moves";
  int color = White;
  bool colorSet = false;
  
  loadFEN(pieceBitboard);
  while(true) {
    getline(std::cin, input);
    if (input == "quit") { return 0; }
    else if (input == "isready") { std::cout << "readyok" << std::endl; }
    else if (input == "uci") { std::cout << "id name quicksand" << std::endl << "id author hardal" << std::endl << "uciok" << std::endl; }

    else if (input.find("position startpos") != std::string::npos) {
      if (!colorSet) {
        (input == "position startpos") ? color = White : color = Black; 
        colorSet = true;
      }
      if (input != "position startpos"){
        std::string opponentMove = input;
        std::string inputRemove = lastCommand + " ";
        std::size_t substringPosition = opponentMove.find(inputRemove);
        if(substringPosition != std::string::npos){
          opponentMove.erase(substringPosition, inputRemove.length());
        }
        else {
          std::cout << ">>::ERROR::IN::UCI::INPUT::EXITING::<<";
          return 0;
        }
        lastCommand = input;

        if      (opponentMove == "e1g1") { makeMove(pieceBitboard, kingSideCastleWhite);  }
        else if (opponentMove == "e1c1") { makeMove(pieceBitboard, queenSideCastleWhite); }
        else if (opponentMove == "e8g8") { makeMove(pieceBitboard, kingSideCastleBlack);  }
        else if (opponentMove == "e8c8") { makeMove(pieceBitboard, queenSideCastleBlack); }
        else { makeMove(pieceBitboard, decodeMove(opponentMove)); }
        visualizeBoard(pieceBitboard);
      }
      
      uint16_t bestMove = searchBestMove(pieceBitboard, color, 3);

      std::cout << "bestmove " << annotateMove(bestMove) << std::endl;
      lastCommand += " " + annotateMove(bestMove);
      makeMove(pieceBitboard, bestMove);
      visualizeBoard(pieceBitboard);
    }
  }
}

int main() {
  startGame();
  return 0;
}
