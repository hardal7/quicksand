#include "move.h"

using namespace Move;

Encoded Move::encode(Readable move) {
  Encoded encoded = NoMove;

  encoded |= move.OriginSquare << OriginSquareOffset;
  encoded |= move.DestinationSquare << DestinationSquareOffset;
  encoded |= move.Flag << FlagOffset;
  encoded |= move.PromotionPiece << PromotionPieceOffset;

  return encoded;
}

Readable Move::decode(Encoded move) {
  Readable decoded;

  decoded.OriginSquare = (move & OriginSquareMask) >> OriginSquareOffset;
  decoded.DestinationSquare = (move & DestinationSquareMask) >> DestinationSquareOffset;
  decoded.Flag = static_cast<Flag>((move & FlagMask) >> FlagOffset);
  decoded.PromotionPiece = static_cast<PromotionPiece>((move & PromotionPieceMask) >> PromotionPieceOffset);

  return decoded;
}

std::string Move::annotation(Readable move) {
  return Board::squareToPosition(move.OriginSquare) + Board::squareToPosition(move.DestinationSquare);
}

std::string Move::annotation(Encoded move) {
  Readable m = decode(move);
  return Board::squareToPosition(m.OriginSquare) + Board::squareToPosition(m.DestinationSquare);
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
