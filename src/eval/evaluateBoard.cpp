#include <bitset>
#include <cmath>
#include <cstdint>

#include "evaluateBoard.hpp"
#include "../utils/enums.hpp"

int evaluatePawnStructure(uint64_t pieceBitboard[8], int square, int pieceColor){
  int evalScore = 0;
  evalScore += (pieceColor == White ? pawnTableWhite[square] : pawnTableBlack[square]);
  if (!((pieceBitboard[Pawn] & pieceBitboard[pieceColor]) & ((1ul << (square+8)) | (1ul >> (square+8)))))
  { evalScore += isolatedPawnPenalty; } 
  if (!((fileMasks[square%8] | fileMasks[((square%8)-1) > 0 ? ((square%8)-1) : (square%8)]
  | fileMasks[((square%8)+1) < 8 ? ((square%8)+1) : (square%8)])
  & (((pieceBitboard[White] | pieceBitboard[Black]) & pieceBitboard[Pawn]) ^ (1ul << square))))
  { evalScore += passedPawnBonus; }  
  if ((fileMasks[square%8] & pieceBitboard[pieceColor] & pieceBitboard[Pawn]) ^ (1ul << square))
  { evalScore += doublePawnPenalty; }
  return evalScore;
}

float smoothBlend(int pieceNumber) {
  if (pieceNumber > 12) { return 1; }
  else if (pieceNumber < 6)  { return 0; }
  else {
    float coefficient = (pieceNumber-6) / 6.0;
    return (coefficient * coefficient) * (3 - (2 * coefficient));
  }
}

float calculateDistance(int firstPosition, int secondPosition) {
  int xDistance = (firstPosition % 8) - (secondPosition % 8);
  int yDistance = (firstPosition / 8) - (secondPosition / 8);
  return (float)sqrt((xDistance * xDistance) + (yDistance * yDistance));
}

int evaluateKing(uint64_t pieceBitboard[8], int square, int color, int eval) {
  float blend = smoothBlend(std::bitset<64>(pieceBitboard[White] | pieceBitboard[Black]).count());
  int evalScore = blend * (color == White ? kingTableWhite : kingTableBlack)[square];
  evalScore += (1 - blend) * (color == White ? kingEndTableWhite : kingEndTableBlack)[square];
  if (!blend && (color == White)) {
    int distanceScore = nearKingBonus * (calculateDistance(square, __builtin_ctzll(pieceBitboard[King] & pieceBitboard[Black])) / 7.0);
    if ((pieceBitboard[White] | pieceBitboard[Black]) & ~(pieceBitboard[Pawn] | pieceBitboard[King])) {
      (eval > 0) ? (evalScore += distanceScore) : (evalScore -= distanceScore);
    }
  }
  return evalScore;
}

int evaluateBoard(uint64_t pieceBitboard[8]){
  int evalScore = 0;

  for (int square=0; square<64; square++){
    for (int pieceType=Pawn; pieceType<=King; pieceType++){
      if (pieceBitboard[pieceType] & (1ul << square)){
        for (int pieceColor=White; pieceColor<=Black; pieceColor+=Black){
          if ((pieceBitboard[pieceType] & pieceBitboard[pieceColor]) & (1ul << square)){
            switch (pieceType) {
              case (Pawn):   
                if (pieceColor == White) { evalScore += evaluatePawnStructure(pieceBitboard, square, White); }
                else { evalScore -= evaluatePawnStructure(pieceBitboard, square, Black); } break;
              case (Knight): evalScore += (pieceColor == White ? 1 : -1) * 
              (pieceColor == White ? knightTableWhite : knightTableBlack)[square]; break;
              case (Bishop): evalScore += (pieceColor == White ? 1 : -1) * 
              (pieceColor == White ? bishopTableWhite : bishopTableBlack)[square]; break;
              case (Rook): evalScore += (pieceColor == White ? 1 : -1) * 
              (pieceColor == White ? rookTableWhite : rookTableBlack)[square]; break;
              case (Queen): evalScore += (pieceColor == White ? 1 : -1) * 
              (pieceColor == White ? queenTableWhite : queenTableBlack)[square]; break;
              case (King):
                if (pieceColor == White) { evalScore += evaluateKing(pieceBitboard, square, White, evalScore); }
                else { evalScore -= evaluateKing(pieceBitboard, square, Black, evalScore); }
            }
            evalScore += (pieceValues[pieceType-1] * (pieceColor == White ? 1 : -1));
          }
        }
      }
    }
  }

  return evalScore;
}
