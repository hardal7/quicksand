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

std::string Move::decode(Encoded move) {
  std::string decoded = "";

  int originSquare = (move & OriginSquareMask) >> OriginSquareOffset;
  int destinationSquare = (move & DestinationSquareMask) >> DestinationSquareOffset;
  int flag = (move & FlagMask) >> FlagOffset;
  int promotionPiece = (move & PromotionPieceMask) >> PromotionPieceOffset;

  decoded += Board::squareToPosition(originSquare);
  decoded += Board::squareToPosition(destinationSquare);

  if (flag == Promotion) {
    char c;
    switch (promotionPiece) {
    case Knight:
      c = 'n';
      break;
    case Bishop:
      c = 'b';
      break;
    case Rook:
      c = 'r';
      break;
    case Queen:
      c = 'q';
      break;
    }
    decoded += c;
  }

  return decoded;
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
