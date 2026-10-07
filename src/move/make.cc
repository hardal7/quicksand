#include "../board/board.h"
#include "../types.h"
#include "move.h"
#include <optional>

using namespace Move;

std::optional<Board::Piece> Move::make(GameState &state, Encoded move) {
  Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];

  Readable m = decode(move);

  std::optional<Board::Piece> capturedPiece = std::nullopt;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if (*opponentPieces & state.Bitboards[piece] & (1ul << m.DestinationSquare)) {
      capturedPiece = piece;

      *opponentPieces &= ~(1ul << m.DestinationSquare);
      state.Bitboards[capturedPiece.value()] &= ~(1ul << m.DestinationSquare);

      break;
    }
  }

  Board::Piece movedPiece = Board::None;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if (*friendlyPieces & state.Bitboards[piece] & (1ul << m.OriginSquare)) {
      movedPiece = piece;

      *friendlyPieces &= ~(1ul << m.OriginSquare);
      state.Bitboards[movedPiece] &= ~(1ul << m.OriginSquare);
      *friendlyPieces |= 1ul << m.DestinationSquare;
      state.Bitboards[movedPiece] |= 1ul << m.DestinationSquare;

      break;
    }
  }

  Board::bitboard friendlyCastlingRank = (state.WhiteToPlay ? Board::RankOne : Board::RankEight);
  switch (m.Flag) {
  case NoFlag:
    break;
  case Promotion: {
    state.Bitboards[Board::Pawn] &= ~(1ul << m.DestinationSquare);

    Board::Index piece = Board::None;
    switch (m.PromotionPiece) {
    case Knight:
      piece = Board::Knight;
      break;
    case Bishop:
      piece = Board::Bishop;
      break;
    case Rook:
      piece = Board::Rook;
      break;
    case Queen:
      piece = Board::Queen;
      break;
    }
    state.Bitboards[piece] |= 1ul << m.DestinationSquare;

    break;
  }
  case Castle: {
    bool isShortCastle = (m.DestinationSquare - m.OriginSquare) == (Right * 2);
    int originSquare = std::__countr_zero(friendlyCastlingRank & (isShortCastle ? Board::FileH : Board::FileA));
    int destinationSquare = originSquare + ((isShortCastle ? (Left * 2) : (Right * 3)));

    *friendlyPieces &= ~(1ul << originSquare);
    state.Bitboards[Board::Rook] &= ~(1ul << originSquare);
    *friendlyPieces |= 1ul << destinationSquare;
    state.Bitboards[Board::Rook] |= 1ul << destinationSquare;

    break;
  }
  case EnPassant: {
    int captureSquare = m.DestinationSquare + (state.WhiteToPlay ? Down : Up);
    *opponentPieces &= ~(1ul << captureSquare);
    state.Bitboards[Board::Pawn] &= ~(1ul << captureSquare);
    break;
  }
  }

  state.enPassantSquarePrev = state.enPassantSquare;
  state.WhiteShortCastlePrev = state.WhiteShortCastle;
  state.WhiteLongCastlePrev = state.WhiteLongCastle;
  state.BlackShortCastlePrev = state.BlackShortCastle;
  state.BlackLongCastlePrev = state.BlackLongCastle;

  bool notMoved = (1ul << m.OriginSquare) & (state.WhiteToPlay ? Board::RankTwo : Board::RankSeven);
  bool doublePawnMove = std::abs(m.OriginSquare - m.DestinationSquare) == (Down * 2);
  if ((movedPiece == Board::Pawn) && notMoved && doublePawnMove) {
    state.enPassantSquare = m.DestinationSquare + (state.WhiteToPlay ? Down : Up);
  } else {
    state.enPassantSquare = 0;
  }

  if (movedPiece == Board::Rook) {
    bool isShortCastleSquare = m.OriginSquare == std::__countr_zero(friendlyCastlingRank & Board::FileH);
    bool isLongCastleSquare = m.OriginSquare == std::__countr_zero(friendlyCastlingRank & Board::FileA);

    if (isShortCastleSquare) {
      state.WhiteToPlay ? state.WhiteShortCastle : state.BlackShortCastle = false;
    } else if (isLongCastleSquare) {
      state.WhiteToPlay ? state.WhiteLongCastle : state.BlackLongCastle = false;
    }
  }

  else if (movedPiece == Board::King) {
    state.WhiteToPlay ? state.WhiteLongCastle : state.BlackLongCastle = false;
    state.WhiteToPlay ? state.WhiteShortCastle : state.BlackShortCastle = false;
  }

  state.WhiteToPlay = !state.WhiteToPlay;
  return capturedPiece;
}

void Move::unmake(GameState &state, Move::Encoded move, std::optional<Board::Piece> capturedPiece) {
  state.WhiteToPlay = !state.WhiteToPlay;

  Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];

  Readable m = Move::decode(move);

  Board::Piece movedPiece;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if (*friendlyPieces & state.Bitboards[piece] & (1ul << m.DestinationSquare)) {
      movedPiece = piece;

      *friendlyPieces &= ~(1ul << m.DestinationSquare);
      state.Bitboards[movedPiece] &= ~(1ul << m.DestinationSquare);
      *friendlyPieces |= 1ul << m.OriginSquare;
      state.Bitboards[movedPiece] |= 1ul << m.OriginSquare;

      break;
    }
  }

  if (capturedPiece.has_value()) {
    if (m.Flag != EnPassant) {
      *opponentPieces |= 1ul << m.DestinationSquare;
      state.Bitboards[capturedPiece.value()] |= 1ul << m.DestinationSquare;
    }
  }

  switch (m.Flag) {
  case NoFlag:
    break;
  case Promotion: {
    state.Bitboards[Board::Pawn] |= 1ul << m.OriginSquare;

    Board::Index piece = Board::Pawn;
    switch (m.PromotionPiece) {
    case Knight:
      piece = Board::Knight;
      break;
    case Bishop:
      piece = Board::Bishop;
      break;
    case Rook:
      piece = Board::Rook;
      break;
    case Queen:
      piece = Board::Queen;
      break;
    }

    state.Bitboards[piece] &= ~(1ul << m.OriginSquare);

    break;
  }
  case Castle: {
    Board::bitboard friendlyCastlingRank = (state.WhiteToPlay ? Board::RankOne : Board::RankEight);
    bool isShortCastle = (m.DestinationSquare - m.OriginSquare) == (Right * 2);
    int originSquare = std::__countr_zero(friendlyCastlingRank & (isShortCastle ? Board::FileH : Board::FileA));
    int destinationSquare = originSquare + ((isShortCastle ? (Left * 2) : (Right * 3)));

    *friendlyPieces &= ~(1ul << destinationSquare);
    state.Bitboards[Board::Rook] &= ~(1ul << destinationSquare);
    *friendlyPieces |= 1ul << originSquare;
    state.Bitboards[Board::Rook] |= 1ul << originSquare;

    break;
  }
  case EnPassant: {
    int captureSquare = m.DestinationSquare + (state.WhiteToPlay ? Down : Up);
    *opponentPieces |= 1ul << captureSquare;
    state.Bitboards[Board::Pawn] |= 1ul << captureSquare;

    break;
  }
  }

  state.enPassantSquare = state.enPassantSquarePrev;
  state.WhiteShortCastle = state.WhiteShortCastlePrev;
  state.WhiteLongCastle = state.WhiteLongCastlePrev;
  state.BlackShortCastle = state.BlackShortCastlePrev;
  state.BlackLongCastle = state.BlackLongCastlePrev;
}
