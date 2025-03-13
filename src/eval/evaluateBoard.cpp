#include <stdint.h>
#include <math.h>

#include "evaluateBoard.hpp"
#include "../utils/enums.hpp"
#include "../utils/popLSB.hpp"

int evaluateBoard(uint64_t bitboards[8], char color){
  uint64_t friendlyBitboard, opponentBitboard, piecePosition;
  int friendlyEval = 0, opponentEval = 0;
  char isEndGame = 0;
  
  //Alternate between pieces (2: Pawn, 7: King) See: "enums.hpp"
  for (unsigned int pieceType = 2; pieceType < 8; pieceType++) {
    //Evaluate for number of pieces
    friendlyEval += (__builtin_popcountll(bitboards[pieceType] & bitboards[color]) * (
    pieceType == Pawn ? pawnValue :pieceType == Knight ? knightValue : pieceType == Bishop ? bishopValue :
    pieceType == Rook ? rookValue : pieceType == Queen ? queenValue : kingValue));
    
    opponentEval += (__builtin_popcountll(bitboards[pieceType] & bitboards[!color]) * (
    pieceType == Pawn ? pawnValue :pieceType == Knight ? knightValue : pieceType == Bishop ? bishopValue :
    pieceType == Rook ? rookValue : pieceType == Queen ? queenValue : kingValue));

    //Evaluate for piece tables
    //Alternate between Friendly (i = 0) and opponent (i=1) Pieces
    for (unsigned int i = 0; i < 2; i++) {
      (i == 0 ? friendlyBitboard : opponentBitboard) = (bitboards[pieceType] & bitboards[(i == 0 ? color : !color)]);
      for (unsigned int j = 0; j < __builtin_popcountll(bitboards[pieceType] & bitboards[(i == 0 ? color : !color)]); j++) {
        piecePosition = log2(popLSB(i == 0 ? friendlyBitboard : opponentBitboard));
        (i == 0 ? friendlyEval : opponentEval) += (
        pieceType == Pawn ? (i == 0 ? pawnTableWhite : pawnTableBlack) :
        pieceType == Knight ? (i == 0 ? knightTableWhite : knightTableBlack):
        pieceType == Bishop ? (i == 0 ? bishopTableWhite : bishopTableBlack):
        pieceType == Rook ? (i == 0 ? rookTableWhite : rookTableBlack): 
        pieceType == Queen ? (i == 0 ? queenTableWhite : queenTableBlack):
        (i == 0 ? kingTableWhite : kingTableBlack))[piecePosition];
      }
    }
  }

  return (friendlyEval - opponentEval);
}
