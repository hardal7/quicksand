#include "../../include/enums.h"
#include "../../include/types.h"
#include <cstdint>
#include <optional>

enum MoveEncodings {
  OriginSquareOffset = 0,
  DestinationSquareOffset = 6,
  FlagOffset = 12,
  PromotionPieceOffset = 14,

  PromotionFlag = 1,
  CastleFlag = 2,
  EnPassantFlag = 3
};

using move = uint16_t;

move createMove(
    int OriginSquare, int destinationSquare,
    std::optional<Board::Piece> promotionPiece = Board::Indexes::None,
    std::optional<bool> isCastle = false) {
  move m = 0;
  m |= OriginSquare << OriginSquareOffset;
  m |= destinationSquare << DestinationSquareOffset;
  if (promotionPiece != Board::Indexes::None) {
    m |= promotionPiece.value() << PromotionPieceOffset;
    m |= PromotionFlag << FlagOffset;
  } else if (isCastle) {
    m |= CastleFlag << FlagOffset;
  }

  return m;
}

void makeMove(GameState &state, move m) {
  int originSquare = m << OriginSquareOffset;
  int destinationSquare = m << DestinationSquareOffset;
  Board::Piece promotionPiece = m << PromotionPieceOffset;
  int flag = m << FlagOffset;

  Board::Piece movingPiece = Board::Indexes::None;
  for (Board::Piece piece = Board::Indexes::Pawn; piece <= Board::Indexes::King;
       piece++) {

    if ((1ul << originSquare) & state.bitboards[piece]) {
      movingPiece = piece;
      break;
    }
  }

  Board::Bitboard *opponentPieces =
      &state.bitboards[state.whiteToPlay ? Board::Indexes::Black
                                         : Board::Indexes::White];
  bool isCapture = (1ul << destinationSquare) & *opponentPieces;
  if (isCapture) {
    Board::Piece capturedPiece = Board::Indexes::None;

    for (Board::Piece piece = Board::Indexes::Pawn;
         piece <= Board::Indexes::King; piece++) {
      if ((1ul << destinationSquare) & state.bitboards[piece]) {
        capturedPiece = piece;
        break;
      }
    }

    *opponentPieces ^= (1ul << destinationSquare);
    state.bitboards[capturedPiece] ^= (1ul << destinationSquare);
  }

  Board::Bitboard *friendlyPieces =
      &state.bitboards[state.whiteToPlay ? Board::Indexes::White
                                         : Board::Indexes::Black];
  *friendlyPieces ^= (1ul << originSquare);
  *friendlyPieces ^= (1ul << destinationSquare);
  state.bitboards[movingPiece] ^= (1ul << originSquare);
  state.bitboards[movingPiece] ^= (1ul << destinationSquare);

  if (movingPiece == Board::Indexes::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = false;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = false;
    }
  }
  bool isDoubleMove =
      (destinationSquare - originSquare) == (state.whiteToPlay ? Up : Down) * 2;
  if ((movingPiece == Board::Indexes::Pawn) && isDoubleMove) {
    state.enPassantSquare = destinationSquare + (state.whiteToPlay ? Down : Up);
  }
  state.whiteToPlay = !state.whiteToPlay;
}
