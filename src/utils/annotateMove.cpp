#include <stdint.h>
#include <stdio.h>

#include "annotateMove.hpp"

char destinationSquare = 0, originSquare = 0;

//Converts a 16-bit move to an UCI compilant move (short algebraic notation)
void annotateMove(uint16_t move){
  char uciMove[6];
  destinationSquare = move & (uint16_t) 0x3F;
  originSquare = (move >> 6) & (uint16_t) 0x3F;

  for (unsigned int i = 0; i < 2; i++) {
    switch ((i == 0 ? destinationSquare : originSquare) % 8) {
      case 0: uciMove[i == 0 ? 2 : 0] = 'a'; break;
      case 1: uciMove[i == 0 ? 2 : 0] = 'b'; break;
      case 2: uciMove[i == 0 ? 2 : 0] = 'c'; break;
      case 3: uciMove[i == 0 ? 2 : 0] = 'd'; break;
      case 4: uciMove[i == 0 ? 2 : 0] = 'e'; break;
      case 5: uciMove[i == 0 ? 2 : 0] = 'f'; break;
      case 6: uciMove[i == 0 ? 2 : 0] = 'g'; break;
      case 7: uciMove[i == 0 ? 2 : 0] = 'h'; break;
      default: printf("::ANNOTATION:ERROR::"); break;
    }
    if ((i == 0 ? destinationSquare : originSquare) < 8) { uciMove[i == 0 ? 3 : 1] = '8';}
    else if ((i == 0 ? destinationSquare : originSquare) < 16) { uciMove[i == 0 ? 3 : 1] = '7';}
    else if ((i == 0 ? destinationSquare : originSquare) < 24) { uciMove[i == 0 ? 3 : 1] = '6';}
    else if ((i == 0 ? destinationSquare : originSquare) < 32) { uciMove[i == 0 ? 3 : 1] = '5';}
    else if ((i == 0 ? destinationSquare : originSquare) < 40) { uciMove[i == 0 ? 3 : 1] = '4';}
    else if ((i == 0 ? destinationSquare : originSquare) < 48) { uciMove[i == 0 ? 3 : 1] = '3';}
    else if ((i == 0 ? destinationSquare : originSquare) < 56) { uciMove[i == 0 ? 3 : 1] = '2';}
    else if ((i == 0 ? destinationSquare : originSquare) < 64) { uciMove[i == 0 ? 3 : 1] = '1';}
  }
  
  //Promotion
  if((move >> 14) == 1){
    char promotionPiece[4] = {'n', 'b', 'r', 'q'};
    uciMove[4] = promotionPiece[(move >> 12)-4];
  }
  
  printf("%s \n", uciMove);
  uciMove[4] = ' ';
}
