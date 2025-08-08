#include <bitset>
#include <cstdint>

#include "evaluateBoard.hpp"
#include "../utils/enums.hpp"

float smoothBlend(int pieceNumber) {
  if (pieceNumber > 16) { return 1; }
  else if (pieceNumber < 10)  { return 0; }
  else {
    float coefficient = (pieceNumber-6) / 6.0;
    return (coefficient * coefficient) * (3 - (2 * coefficient));
  }
}

int evaluatePawnStructure(uint64_t pieceBitboard[8], int square, int pieceColor){
  int evalScore = 0;
  if (!((pieceBitboard[Pawn] & pieceBitboard[pieceColor]) & ((1ul << (square+8)) | (1ul >> (square+8)))))
  { evalScore += isolatedPawnPenalty; } 
  if (!((fileMasks[square%8] | fileMasks[((square%8)-1) > 0 ? ((square%8)-1) : (square%8)]
  | fileMasks[((square%8)+1) < 8 ? ((square%8)+1) : (square%8)])
  & (((pieceBitboard[White] | pieceBitboard[Black]) & pieceBitboard[Pawn]) ^ (1ul << square))))
  { evalScore += passedPawnWeight; }  
  if ((fileMasks[square%8] & pieceBitboard[pieceColor] & pieceBitboard[Pawn]) ^ (1ul << square))
  { evalScore += doublePawnPenalty; }
  return evalScore;
}

int evaluateBoard(uint64_t pieceBitboard[8]){
  int evalScore = 0;
  float gameState = smoothBlend(std::bitset<64>(pieceBitboard[White] | pieceBitboard[Black]).count());

  for (int square=0; square<64; square++){
    for (int pieceType=Pawn; pieceType<=King; pieceType++){
      if (pieceBitboard[pieceType] & (1ul << square)){
        for (int pieceColor=White; pieceColor<=Black; pieceColor+=Black){
          if ((pieceBitboard[pieceType] & pieceBitboard[pieceColor]) & (1ul << square)){
            switch (pieceType) {
              case (Pawn):   
                evalScore += (pieceColor == White ? 1 : -1) * evaluatePawnStructure(pieceBitboard, square, pieceColor);
                evalScore += (pieceColor == White ? 1 : -1) * ((gameState * (mgPawnTable[pieceColor == White ? (square) : (square ^ 56)]))
                + ((1 - gameState) * egPawnTable[pieceColor == White ? (square) : (square ^ 56)])); break;
              case (Knight):
                evalScore += (pieceColor == White ? 1 : -1) * ((gameState * (mgKnightTable[pieceColor == White ? (square) : (square ^ 56)]))
                + ((1 - gameState) * egKnightTable[pieceColor == White ? (square) : (square ^ 56)])); break;
              case (Bishop):
                evalScore += (pieceColor == White ? 1 : -1) * ((gameState * (mgBishopTable[pieceColor == White ? (square) : (square ^ 56)]))
                + ((1 - gameState) * egBishopTable[pieceColor == White ? (square) : (square ^ 56)])); break;
              case (Rook):
                evalScore += (pieceColor == White ? 1 : -1) * ((gameState * (mgRookTable[pieceColor == White ? (square) : (square ^ 56)]))
                + ((1 - gameState) * egRookTable[pieceColor == White ? (square) : (square ^ 56)])); break;
              case (Queen):
                evalScore += (pieceColor == White ? 1 : -1) * ((gameState * (mgQueenTable[pieceColor == White ? (square) : (square ^ 56)]))
                + ((1 - gameState) * egQueenTable[pieceColor == White ? (square) : (square ^ 56)])); break;
              case (King):
                evalScore += (pieceColor == White ? 1 : -1) * ((gameState * (mgKingTable[pieceColor == White ? (square) : (square ^ 56)]))
                + ((1 - gameState) * egKingTable[pieceColor == White ? (square) : (square ^ 56)])); break;
              }
              evalScore +=  gameState * mgPieceValues[pieceType-1] * (pieceColor == White ? 1 : -1);
              evalScore +=  (1 - gameState) * egPieceValues[pieceType-1] * (pieceColor == White ? 1 : -1);
            }
          }
        }
      }
    }

  return evalScore;
}
