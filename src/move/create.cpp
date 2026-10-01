#include "../../include/enums.h"
#include "../../include/types.h"
#include "castle.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <optional>

enum MoveEncodings {
  OriginSquareOffset = 0,
  DestinationSquareOffset = 6,
  FlagOffset = 12,
  PromotionPieceOffset = 14,

  OriginSquareOffsetMask = 0x3F,
  DestinationSquareOffsetMask = 0xFC0,
  FlagOffsetMask = 0x3000,
  PromotionPieceOffsetMask = 0xC00,

  PromotionFlag = 1,
  CastleFlag = 2,
  EnPassantFlag = 3
};

using move = uint16_t;

// TODO: Set En Passant Flag
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

  std::cout << "From: " << originSquare << " To: " << destinationSquare
            << " Flag: " << flag << std::endl;

  *friendlyPieces ^= (1ul << originSquare);
  *friendlyPieces ^= (1ul << destinationSquare);

  switch (flag) {
  case PromotionFlag: {
    state.bitboards[Board::Pawn] ^= (1ul << originSquare);
    state.bitboards[promotionPiece] ^= (1ul << destinationSquare);

    return capturedPiece;
  }
  case CastleFlag: {
    state.bitboards[Board::King] ^= (1ul << originSquare);
    state.bitboards[Board::King] ^= (1ul << destinationSquare);

    bool isShortCastle = true;
    if (destinationSquare - originSquare == Left * 3) {
      isShortCastle = false;
    }

    int rookOriginSquare = shortCastleRookPosition(state.whiteToPlay);
    int rookDestinationSquare =
        rookOriginSquare + (isShortCastle ? Right * 2 : Left * 3);
    *friendlyPieces ^= (1ul << rookOriginSquare);
    *friendlyPieces ^= (1ul << rookDestinationSquare);
    state.bitboards[Board::Rook] ^= (1ul << rookOriginSquare);
    state.bitboards[Board::Rook] ^= (1ul << rookDestinationSquare);

    return capturedPiece;
  }
  case EnPassantFlag: {
    state.bitboards[Board::Pawn] ^= (1ul << originSquare);
    state.bitboards[Board::Pawn] ^= (1ul << destinationSquare);

    int capturedPawnSquare =
        destinationSquare + (state.whiteToPlay ? Down : Up);
    state.bitboards[Board::Pawn] ^= (1ul << capturedPawnSquare);
    *opponentPieces ^= (1ul << capturedPawnSquare);

    return Board::Pawn;
  }
  }

  bool isCapture = (1ul << destinationSquare) & *opponentPieces;
  if (isCapture) {
    std::cout << "Is capture" << std::endl;

    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if ((1ul << destinationSquare) & state.bitboards[piece]) {
        capturedPiece = piece;
        *opponentPieces ^= (1ul << destinationSquare);
        state.bitboards[capturedPiece] ^= (1ul << destinationSquare);
        break;
      }
    }
  }

  Board::Piece movingPiece = Board::None;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if ((1ul << originSquare) & state.bitboards[piece]) {
      movingPiece = piece;
      std::cout << "Piece: " << movingPiece << std::endl;
      state.bitboards[movingPiece] ^= (1ul << originSquare);
      state.bitboards[movingPiece] ^= (1ul << destinationSquare);
      break;
    }
  }

  if (movingPiece == Board::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = false;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = false;
    }
  }
  bool isDoubleMove =
      (destinationSquare - originSquare) == (state.whiteToPlay ? Up : Down) * 2;
  if ((movingPiece == Board::Pawn) && isDoubleMove) {
    state.enPassantSquare = destinationSquare + (state.whiteToPlay ? Down : Up);
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

  *friendlyPieces ^= (1ul << originSquare);
  *friendlyPieces ^= (1ul << destinationSquare);

  switch (flag) {
  case PromotionFlag: {
    state.bitboards[Board::Pawn] ^= (1ul << originSquare);
    state.bitboards[promotionPiece] ^= (1ul << destinationSquare);
    return;
  }
  case CastleFlag: {
    state.bitboards[Board::King] ^= (1ul << originSquare);
    state.bitboards[Board::King] ^= (1ul << destinationSquare);

    bool isShortCastle = true;
    if (destinationSquare - originSquare == Left * 3) {
      isShortCastle = false;
    }

    int rookOriginSquare = shortCastleRookPosition(state.whiteToPlay);
    int rookDestinationSquare =
        rookOriginSquare + (isShortCastle ? Right * 2 : Left * 3);
    *friendlyPieces ^= (1ul << rookOriginSquare);
    *friendlyPieces ^= (1ul << rookDestinationSquare);
    state.bitboards[Board::Rook] ^= (1ul << rookOriginSquare);
    state.bitboards[Board::Rook] ^= (1ul << rookDestinationSquare);

    return;
  }
  case EnPassantFlag: {
    state.bitboards[Board::Pawn] ^= (1ul << originSquare);
    state.bitboards[Board::Pawn] ^= (1ul << destinationSquare);

    int capturedPawnSquare =
        destinationSquare + (state.whiteToPlay ? Down : Up);
    state.bitboards[Board::Pawn] ^= (1ul << capturedPawnSquare);
    *opponentPieces ^= (1ul << capturedPawnSquare);

    return;
  }
  }

  if (capturedPiece != Board::None) {
    *opponentPieces ^= (1ul << destinationSquare);
    state.bitboards[capturedPiece] ^= (1ul << destinationSquare);
  }

  Board::Piece movingPiece = Board::None;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if ((1ul << destinationSquare) & state.bitboards[piece]) {
      movingPiece = piece;
      state.bitboards[movingPiece] ^= (1ul << originSquare);
      state.bitboards[movingPiece] ^= (1ul << destinationSquare);
      break;
    }
  }

  if (movingPiece == Board::Rook) {
    if (originSquare % 8 == fileH) {
      state.canShortCastle = true;
    } else if (originSquare % 8 == fileA) {
      state.canLongCastle = true;
    }
  }
  state.enPassantSquare = 0;
}
