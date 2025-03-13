#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "viewBoard.hpp"
#include "enums.hpp"

const char pieceSymbols[12] = {'p', 'n', 'b', 'r', 'q', 'k', 'P', 'N', 'B', 'R', 'Q', 'K'};

void visualizeBoard(uint64_t bitboards[8], char castlingRights, char color) {
  char boardSquares[64];
  memset(boardSquares, ' ', sizeof(boardSquares));

  //Iterate for 6 different piece types, offset is set accordingly to: "enums.hpp"
  for(unsigned int i = 0; i < 6; i++){
    for(unsigned int j = 0; j < 64; j++){
      if(bitboards[i + 2] & (1ul << j)){
        char pieceColor = !((1ul << j) & bitboards[White]);
        boardSquares[j] = pieceSymbols[i + (pieceColor == White ? 0 : 6)];
      }
    }
  }

  char castlingString[8] = "";
  for (unsigned int i = 0; i < 4; i++){
    if(castlingRights & (i == 0 ? 1 : i == 1 ? 2 : i == 2 ? 4 : 8))
    { strcat(castlingString, (i == 0 ? "s" : i == 1 ? "l" : i == 2 ? "S" : "L")); }
  }
  printf("\nSide: %s | Castling Rights: %s\n", (color == White ? "White" : "Black"), 
  (castlingRights == 0 ? "None" : castlingString));

  printf("-----------------------------------\n");
  for(unsigned int squareIndex = 0; squareIndex < 64; squareIndex++){ 
    if(squareIndex % 8 == 0){ printf("#|"); }
    printf(" %c |", boardSquares[squareIndex]);
    if(squareIndex % 8 == 7){ printf("#\n-----------------------------------\n"); }
  }
}
