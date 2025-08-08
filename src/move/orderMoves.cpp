#include <cstdint>

#include "../utils/enums.hpp"

void orderMoves (uint64_t pieceBitboard[8], uint16_t movesList[256], int movesNum) {
  int captureMovesNum = 0;
  int regularMovesNum = 0;
  for (int moveType=0; moveType<2; moveType++){
    for (int moveIndex=128; moveIndex<(movesNum+128); moveIndex++){
      bool captureMove = (1ul << (movesList[moveIndex] & 0x3F)) & (pieceBitboard[White] | pieceBitboard[Black]);
      if (moveType ? (!captureMove) : captureMove) {
        movesList[captureMovesNum + (moveType ? regularMovesNum : 0)] = movesList[moveIndex]; 
        if (moveType) { regularMovesNum++; }
        else { captureMovesNum++; }
      }
    }
  }
}
