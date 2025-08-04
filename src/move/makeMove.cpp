#include <cstdint>

#include "../utils/enums.hpp"

int makeMove(uint64_t pieceBitboard[8], uint16_t move, bool unmakeMove = 0, int capturedPiece = 0){
  const uint64_t destinationSquare = 1ul << (move & 0x3F);
  const uint64_t originSquare = 1ul << ((move & 0xFC0) >> 6);
  const int moveFlag = ((move & 0x3000) >> 12);
  const int promotionPiece = (((move & 0xC000) >> 14) + 2);

  if (moveFlag == Castling) {
    switch (move) {
      case kingSideCastleWhite: 
        (pieceBitboard[Rook])  ^= ((1ul << 63) | (1ul << 61));
        (pieceBitboard[White]) ^= ((1ul << 63) | (1ul << 61)); break;
      case queenSideCastleWhite: 
        (pieceBitboard[Rook])  ^= ((1ul << 56) | (1ul << 59));
        (pieceBitboard[White]) ^= ((1ul << 56) | (1ul << 59)); break;
      case kingSideCastleBlack: 
        (pieceBitboard[Rook])  ^= ((1ul << 7) | (1ul << 5));
        (pieceBitboard[Black]) ^= ((1ul << 7) | (1ul << 5)); break;
      case queenSideCastleBlack: 
        (pieceBitboard[Rook])  ^= ((1ul << 0) | (1ul << 3));
        (pieceBitboard[Black]) ^= ((1ul << 0) | (1ul << 3)); break;
    }
  }
  
  int movingPiece = 0;
  for (int pieceColor=White; pieceColor<=Black; pieceColor+=Black){
    for (int pieceType=Pawn; pieceType<=King; pieceType++) { 
      if((originSquare & pieceBitboard[pieceColor] & pieceBitboard[pieceType]) && !unmakeMove){
        movingPiece = (pieceColor + pieceType);
      }
      if(destinationSquare & pieceBitboard[pieceColor] & pieceBitboard[pieceType]){
        (unmakeMove ? movingPiece : capturedPiece) = (pieceColor + pieceType);
      }
    }
  } 

  pieceBitboard[movingPiece < Black ? White : Black] ^= (originSquare | destinationSquare);
  pieceBitboard[movingPiece < Black ? movingPiece : (movingPiece-Black)] ^= (originSquare | destinationSquare);

  if (capturedPiece) {
    pieceBitboard[capturedPiece < Black ? White : Black] ^= destinationSquare;
    pieceBitboard[capturedPiece < Black ? capturedPiece : (capturedPiece-Black)] ^= destinationSquare;
  }
  if (moveFlag == Promotion) {
    if (!unmakeMove) {
      pieceBitboard[movingPiece < Black ? movingPiece : (movingPiece-Black)] ^= destinationSquare;
      pieceBitboard[promotionPiece] ^= destinationSquare;
    }
    else {
      pieceBitboard[movingPiece < Black ? movingPiece : (movingPiece-Black)] ^= originSquare;
      pieceBitboard[Pawn] ^= originSquare;
    }
  }
  
  return capturedPiece;
}
