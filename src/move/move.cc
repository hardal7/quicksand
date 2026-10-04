#include "move.h"

using namespace Move;

Encoded Move::encode(Readable move) {
  Encoded encoded = NoMove;

  encoded |= ((move.OriginSquare << OriginSquareOffset) & OriginSquareMask);
  encoded |= ((move.DestinationSquare << DestinationSquareOffset) & DestinationSquareMask);
  encoded |= ((move.Flag << FlagOffset) & FlagMask);
  encoded |= ((move.PromotionPiece << PromotionPieceOffset) & PromotionPieceMask);

  return encoded;
}

void Move::insert(List &movesList, Readable m) {
  int move = m.DestinationSquare - m.OriginSquare;
  if (Constraints.find(move) != Constraints.end()) {
    Board::bitboard constraintMask = Constraints.at(move);
    bool moveConstrained = constraintMask & (1ul << m.OriginSquare);
    if (moveConstrained) {
      return;
    }
  }

  if (m.Flag != Promotion) {
    movesList.list[movesList.length] = encode(m);
    movesList.length++;
  }

  else {
    for (Move::PromotionPiece piece : PromotionPieces) {
      m.PromotionPiece = piece;
      movesList.list[movesList.length] = encode(m);
      movesList.length++;
    }
  }
}
