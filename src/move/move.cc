#include "move.h"
#include <cstdlib>

using namespace Move;

Encoded Move::encode(Readable move) {
  Encoded encoded = NoMove;

  encoded |= move.OriginSquare << OriginSquareOffset;
  encoded |= move.DestinationSquare << DestinationSquareOffset;
  encoded |= move.Flag << FlagOffset;
  encoded |= move.PromotionPiece << PromotionPieceOffset;

  return encoded;
}

Encoded Move::encode(GameState state, std::string move) {
  Readable m;
  m.OriginSquare = Board::squareFromPosition(move.substr(0, 2));
  m.DestinationSquare = Board::squareFromPosition(move.substr(2, 2));

  if (move.length() == 5) {
    m.Flag = Promotion;
    switch (move[4]) {
    case 'n':
      m.PromotionPiece = Knight;
      break;
    case 'b':
      m.PromotionPiece = Bishop;
      break;
    case 'r':
      m.PromotionPiece = Rook;
      break;
    case 'q':
      m.PromotionPiece = Queen;
      break;
    }
  }

  Board::bitboard opponentPieces = state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];
  bool isPawnMove = (1ul << m.OriginSquare) & state.Bitboards[Board::Pawn];
  int direction = std::abs(m.OriginSquare - m.DestinationSquare);
  bool isPawnCapture = (direction == Up + Left) || (direction == Up + Right);
  bool pieceOnCapture = (1ul << m.DestinationSquare) & opponentPieces;
  if (isPawnMove && isPawnCapture && !pieceOnCapture) {
    m.Flag = EnPassant;
  }

  bool isKingMove = (1ul << m.OriginSquare) & state.Bitboards[Board::King];
  direction = m.DestinationSquare - m.OriginSquare;
  bool isCastle = (direction == Left * 2) || (direction == Right * 2);
  if (isKingMove && isCastle) {
    m.Flag = Castle;
  }

  return encode(m);
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
