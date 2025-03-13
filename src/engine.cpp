#include <stdio.h>
#include <stdint.h>

#include "utils/loadFen.hpp"
#include "utils/annotateMove.hpp"
#include "utils/viewBoard.hpp"
#include "move/generateMoves.hpp"
#include "move/makeMove.hpp"
#include "move/unmakeMove.hpp"
#include "eval/evaluateBoard.hpp"

int main()
{
  uint64_t bitboards[8] = {0};
  uint64_t bitboardsCopy[8] = {0};
  uint16_t movesList[128] = {0};
  uint16_t enPassantSquares = 0;
  char castlingRights = 0b1111; //All forms of castling are allowed initially
  char color = White;

  loadFEN(bitboards, (char *)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  //Exampler Positions:
  //loadFEN(bitboards, (char*)"rnbqkbnr/pppppppp/8/8/2BPPBQ1/2N2N2/PPP2PPP/R3K2R");
  //loadFEN(bitboards, (char*)"rnbqkbnr/pppppPpp/8/8/2BP1BQ1/2N2N2/PPP2PPP/R3K2R");
  //loadFEN(bitboards, (char *)"r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
  //loadFEN(bitboards, (char *)"4k2r/6r1/8/8/8/8/3R4/R3K3");

  int evaluation = evaluateBoard(bitboards, White);
  //visualizeBoard(bitboards, castlingRights, White);
  //printf("Evaluation: %d\n", evaluation);
  unsigned int movesNum = generatePsuedoLegalMoves(bitboards, movesList, enPassantSquares, castlingRights, color);
  
  visualizeBoard(bitboards, castlingRights, White);
  printf("Evaluation: %d\n\n", evaluation);

  for (unsigned int i = 0; i < movesNum; i++) { 
    //for (unsigned int j = 0; j < 8; j++) {bitboardsCopy[j]=bitboards[j];}
    //printf("(Move %d): ", i);
    annotateMove(movesList[i]);
    //makeMove(bitboards, movesList[i], color);
    //visualizeBoard(bitboards, castlingRights, White);
    //evaluation = evaluateBoard(bitboards, White);
    //printf("Evaluation: %d\n\n", evaluation);
    //makeMove(bitboards, movesList[i], color);
  }

  return 0;
}
