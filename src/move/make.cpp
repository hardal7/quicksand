#include "make.h"
#include "../board/board.h"
#include "../board/print.h"
#include "castle.h"
#include "encode.h"
#include "enums.h"
#include <algorithm>

void createMove(int originSquare, int destinationSquare,
                std::array<move, MAX_MOVES> &movesList, moveType t) {
  move m = 0;
  m |= originSquare << OriginSquareOffset;
  m |= destinationSquare << DestinationSquareOffset;

  if (t.type == SpecialMove::Promotion) {
    m |= PromotionFlag << FlagOffset;
    m |= t.promotionPiece << PromotionPieceOffset;
  } else if (t.type == SpecialMove::Castle) {
    m |= CastleFlag << FlagOffset;
  } else if (t.type == SpecialMove::EnPassant) {
    m |= EnPassantFlag << FlagOffset;
  }

  auto it = std::find(movesList.begin(), movesList.end(), NO_MOVE);
  *it = m;
}

Board::Piece makeMove(GameState &state, move m) {
  int originSquare = (m & OriginSquareOffsetMask) >> OriginSquareOffset;
  int destinationSquare =
      (m & DestinationSquareOffsetMask) >> DestinationSquareOffset;
  Board::Piece promotionPiece =
      (m & PromotionPieceOffsetMask) >> PromotionPieceOffset;
  int flag = (m & FlagOffsetMask) >> FlagOffset;

  if (flag == PromotionFlag) {
    printBitboards(state.bitboards);
    printBoard(state);
  }

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
  case PromotionFlag: {
    state.bitboards[Board::Pawn] &= ~(1ul << destinationSquare);
    state.bitboards[promotionPiece] |= (1ul << destinationSquare);
  } break;
  case EnPassantFlag: {
    int enPassantCaptureSquare =
        destinationSquare + (state.whiteToPlay ? Down : Up);

    int testVar = std::abs(destinationSquare - originSquare);
    if (testVar != 7 && testVar != 9 && capturedPiece != Board::None) {
      printBoard(state);
    }

    *opponentPieces &= ~(1ul << enPassantCaptureSquare);
    state.bitboards[Board::Pawn] &= ~(1ul << enPassantCaptureSquare);

    capturedPiece = Board::Pawn;
  } break;
  case CastleFlag: {
    bool isShortCastle = true;
    if (destinationSquare - originSquare == Left * 3) {
      isShortCastle = false;
    }

    int rookOriginSquare =
        (isShortCastle ? shortCastleRookSquare(state.whiteToPlay)
                       : longCastleRookSquare(state.whiteToPlay));
    int rookDestinationSquare =
        rookOriginSquare + (isShortCastle ? (Left * 2) : (Right * 3));

    state.bitboards[Board::Rook] &= ~(1ul << rookOriginSquare);
    state.bitboards[Board::Rook] |= (1ul << rookDestinationSquare);
    *friendlyPieces &= ~(1ul << rookOriginSquare);
    *friendlyPieces |= (1ul << rookDestinationSquare);

    state.canLongCastle = false;
    state.canShortCastle = false;
  } break;
  }

  if (movedPiece == Board::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = false;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = false;
    }
  }

  state.enPassantSquarePrev = state.enPassantSquare;
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

  if (capturedPiece != Board::None && flag != EnPassantFlag) {
    *opponentPieces |= (1ul << destinationSquare);
    state.bitboards[capturedPiece] |= (1ul << destinationSquare);
  }

  switch (flag) {
  case PromotionFlag: {
    state.bitboards[promotionPiece] &= ~(1ul << originSquare);
    state.bitboards[Board::Pawn] |= (1ul << originSquare);
  } break;
  case EnPassantFlag: {
    int enPassantCaptureSquare =
        destinationSquare + (state.whiteToPlay ? Down : Up);
    *opponentPieces |= (1ul << enPassantCaptureSquare);
    state.bitboards[Board::Pawn] |= (1ul << enPassantCaptureSquare);
  } break;
  case CastleFlag: {
    bool isShortCastle = true;
    if (destinationSquare - originSquare == Left * 3) {
      isShortCastle = false;
    }

    int rookOriginSquare =
        (isShortCastle ? shortCastleRookSquare(state.whiteToPlay)
                       : longCastleRookSquare(state.whiteToPlay));
    int rookDestinationSquare =
        rookOriginSquare + (isShortCastle ? (Left * 2) : (Right * 3));

    state.bitboards[Board::Rook] &= ~(1ul << rookDestinationSquare);
    state.bitboards[Board::Rook] |= (1ul << rookOriginSquare);
    *friendlyPieces &= ~(1ul << rookDestinationSquare);
    *friendlyPieces |= (1ul << rookOriginSquare);

    state.canLongCastle = true;
    state.canShortCastle = true;
  } break;
  }

  if (movedPiece == Board::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = true;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = true;
    }
  }

  state.enPassantSquare = state.enPassantSquarePrev;
}
