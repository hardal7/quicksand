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

void Move::insert(List &list, Readable m) {
  if (m.Flag != Promotion) {
    auto it = std::find(list.begin(), list.end(), NoMove);
    int index = it - list.begin();
    list[index] = encode(m);
  }

  else {
    for (Move::PromotionPiece piece : PromotionPieces) {
      auto it = std::find(list.begin(), list.end(), NoMove);
      int index = it - list.begin();
      m.PromotionPiece = piece;
      list[index] = encode(m);
    }
  }
}
