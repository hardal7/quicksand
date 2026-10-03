#include "../board/board.h"
#include "castle.h"
#include "encode.h"
#include "enums.h"
#include <algorithm>
#include <optional>

void createMove(int originSquare, int destinationSquare,
                std::array<move, MAX_MOVES> &movesList,
                std::optional<Board::Piece> promotionPiece = Board::None,
                std::optional<bool> isCastle = false) {
  move m = 0;
  m |= originSquare << OriginSquareOffset;
  m |= destinationSquare << DestinationSquareOffset;
  if (promotionPiece != Board::None) {
    m |= promotionPiece.value() << PromotionPieceOffset;
    m |= PromotionFlag << FlagOffset;
  } else if (isCastle.value()) {
    m |= CastleFlag << FlagOffset;
  }

  auto it = std::find(movesList.begin(), movesList.end(), 0);
  *it = m;
}

Board::Piece makeMove(GameState &state, move m) {
  int originSquare = (m & OriginSquareOffsetMask) >> OriginSquareOffset;
  int destinationSquare =
      (m & DestinationSquareOffsetMask) >> DestinationSquareOffset;
  Board::Piece promotionPiece =
      (m & PromotionPieceOffsetMask) >> PromotionPieceOffset;
  int flag = (m & FlagOffsetMask) >> FlagOffset;

  Board::Bitboard *friendlyPieces =
      &state.bitboards[state.whiteToPlay ? Board::White : Board::Black];
  Board::Bitboard *opponentPieces =
      &state.bitboards[state.whiteToPlay ? Board::Black : Board::White];

  Board::Piece capturedPiece = Board::None;
  if (*opponentPieces & (1ul << destinationSquare)) {
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (*opponentPieces & state.bitboards[piece] &
          (1ul << destinationSquare)) {
        capturedPiece = piece;

        *opponentPieces &= ~(1ul << destinationSquare);
        state.bitboards[capturedPiece] &= ~(1ul << destinationSquare);
      }
    }
  }

  Board::Piece movedPiece = Board::None;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if (*friendlyPieces & state.bitboards[piece] & (1ul << originSquare)) {
      movedPiece = piece;
    }
  }

  *friendlyPieces &= ~(1ul << originSquare);
  *friendlyPieces |= (1ul << destinationSquare);
  state.bitboards[movedPiece] &= ~(1ul << originSquare);
  state.bitboards[movedPiece] |= (1ul << destinationSquare);

  switch (flag) {
  case PromotionFlag:
    state.bitboards[Board::Pawn] &= ~(1ul << destinationSquare);
    state.bitboards[promotionPiece] |= (1ul << destinationSquare);
  case EnPassantFlag: {
    int enPassantCaptureSquare =
        destinationSquare + (state.whiteToPlay ? Up : Down);
    *opponentPieces &= ~(1ul << enPassantCaptureSquare);
    state.bitboards[Board::Pawn] &= ~(1ul << enPassantCaptureSquare);
    capturedPiece = Board::Pawn;
  }
  case CastleFlag: {
    bool isShortCastle = true;
    if (destinationSquare - originSquare == Left * 3) {
      isShortCastle = false;
    }

    int rookOriginSquare =
        (isShortCastle ? shortCastleRookPosition(state.whiteToPlay)
                       : longCastleRookPosition(state.whiteToPlay));
    int rookDestinationSquare =
        rookOriginSquare + (isShortCastle ? (Right * 2) : (Left * 3));

    state.bitboards[Board::Rook] &= ~(1ul << rookOriginSquare);
    state.bitboards[Board::Rook] |= (1ul << rookDestinationSquare);
    *friendlyPieces &= ~(1ul << rookOriginSquare);
    *friendlyPieces |= (1ul << rookDestinationSquare);

    state.canLongCastle = false;
    state.canShortCastle = false;
  }
  }

  if (movedPiece == Board::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = false;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = false;
    }
  }

  bool isDoubleMove =
      (destinationSquare - originSquare) == (state.whiteToPlay ? Up : Down) * 2;
  if ((movedPiece == Board::Pawn) && isDoubleMove) {
    state.enPassantSquare = destinationSquare + (state.whiteToPlay ? Down : Up);
  } else {
    state.enPassantSquare = 0;
  }

  state.whiteToPlay = !state.whiteToPlay;

  return capturedPiece;
}

void unmakeMove(GameState &state, move m, Board::Piece capturedPiece) {
  int originSquare = (m & OriginSquareOffsetMask) >> OriginSquareOffset;
  int destinationSquare =
      (m & DestinationSquareOffsetMask) >> DestinationSquareOffset;
  Board::Piece promotionPiece =
      (m & PromotionPieceOffsetMask) >> PromotionPieceOffset;
  int flag = (m & FlagOffsetMask) >> FlagOffset;

  state.whiteToPlay = !state.whiteToPlay;

  Board::Bitboard *friendlyPieces =
      &state.bitboards[state.whiteToPlay ? Board::White : Board::Black];
  Board::Bitboard *opponentPieces =
      &state.bitboards[state.whiteToPlay ? Board::Black : Board::White];

  Board::Piece movedPiece = Board::None;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if (*friendlyPieces & state.bitboards[piece] & (1ul << destinationSquare)) {
      movedPiece = piece;
    }
  }

  *friendlyPieces &= ~(1ul << destinationSquare);
  *friendlyPieces |= (1ul << originSquare);
  state.bitboards[movedPiece] &= ~(1ul << destinationSquare);
  state.bitboards[movedPiece] |= (1ul << originSquare);

  if (capturedPiece != Board::None) {
    *opponentPieces |= (1ul << destinationSquare);
    state.bitboards[capturedPiece] |= (1ul << destinationSquare);
  }

  switch (flag) {
  case PromotionFlag:
    state.bitboards[promotionPiece] &= ~(1ul << originSquare);
    state.bitboards[Board::Pawn] |= (1ul << originSquare);
  case EnPassantFlag: {
    int enPassantCaptureSquare =
        destinationSquare + (state.whiteToPlay ? Up : Down);
    *opponentPieces |= (1ul << enPassantCaptureSquare);
    state.bitboards[Board::Pawn] |= (1ul << enPassantCaptureSquare);
  }
  case CastleFlag: {
    bool isShortCastle = true;
    if (destinationSquare - originSquare == Left * 3) {
      isShortCastle = false;
    }

    int rookOriginSquare =
        (isShortCastle ? shortCastleRookPosition(state.whiteToPlay)
                       : longCastleRookPosition(state.whiteToPlay));
    int rookDestinationSquare =
        rookOriginSquare + (isShortCastle ? (Right * 2) : (Left * 3));

    state.bitboards[Board::Rook] &= ~(1ul << rookDestinationSquare);
    state.bitboards[Board::Rook] |= (1ul << rookOriginSquare);
    *friendlyPieces &= ~(1ul << rookDestinationSquare);
    *friendlyPieces |= (1ul << rookOriginSquare);

    state.canLongCastle = true;
    state.canShortCastle = true;
  }
  }

  if (movedPiece == Board::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = true;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = true;
    }
  }

  bool isDoubleMove =
      (destinationSquare - originSquare) == (state.whiteToPlay ? Up : Down) * 2;
  if ((movedPiece == Board::Pawn) && isDoubleMove) {
    state.enPassantSquare = destinationSquare + (state.whiteToPlay ? Down : Up);
  } else {
    state.enPassantSquare = 0;
  }
}
