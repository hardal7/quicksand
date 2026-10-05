#include "../board/board.h"
#include "../types.h"
#include "move.h"
#include <optional>

std::optional<Board::Piece> makeMove(GameState &state, Move::Encoded move) {
  Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];

  Move::Readable m = Move::decode(move);

  std::optional<Board::Piece> capturedPiece = std::nullopt;
  for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
    if (*opponentPieces & state.Bitboards[piece] & (1ul << m.DestinationSquare)) {
      capturedPiece = piece;

      *opponentPieces &= ~(1ul << m.DestinationSquare);
      state.Bitboards[capturedPiece.value()] &= ~(1ul << m.DestinationSquare);

      break;
    }
  }

  Board::Piece movedPiece;
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
  case Move::NoFlag:
    break;
  case Move::Promotion: {
    state.Bitboards[Board::Pawn] &= ~(1ul << m.DestinationSquare);

    Board::Index piece;
    switch (m.PromotionPiece) {
    case Move::Knight:
      piece = Board::Knight;
      break;
    case Move::Bishop:
      piece = Board::Bishop;
      break;
    case Move::Rook:
      piece = Board::Rook;
      break;
    case Move::Queen:
      piece = Board::Queen;
      break;
    }
    state.Bitboards[piece] |= 1ul << m.DestinationSquare;

    break;
  }
  case Move::Castle: {
    bool isShortCastle = (m.DestinationSquare - m.OriginSquare) == (Move::Right * 2);
    int originSquare = std::__countr_zero(friendlyCastlingRank & (isShortCastle ? Board::FileA : Board::FileH));
    int destinationSquare = originSquare + (Move::Right * 2 * (isShortCastle ? 1 : -1));

    *friendlyPieces &= ~(1ul << originSquare);
    state.Bitboards[Board::Rook] &= ~(1ul << originSquare);
    *friendlyPieces |= 1ul << destinationSquare;
    state.Bitboards[Board::Rook] |= 1ul << destinationSquare;

    break;
  }
  case Move::EnPassant: {
    int captureSquare = m.DestinationSquare + (state.WhiteToPlay ? Move::Down : Move::Up);
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
  if ((movedPiece == Board::Pawn) && notMoved) {
    state.enPassantSquare = m.DestinationSquare + (state.WhiteToPlay ? Move::Up : Move::Down);
  }

  else if (movedPiece == Board::Rook) {
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

void unmakeMove(GameState &state, Move::Encoded move, std::optional<Board::Piece> capturedPiece) {
  state.WhiteToPlay = !state.WhiteToPlay;

  Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];

  Move::Readable m = Move::decode(move);

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

  if (capturedPiece != std::nullopt) {
    if (m.Flag != Move::EnPassant) {
      *opponentPieces |= 1ul << m.DestinationSquare;
      state.Bitboards[capturedPiece.value()] |= 1ul << m.DestinationSquare;
    }
  }

  Board::bitboard friendlyCastlingRank = (state.WhiteToPlay ? Board::RankOne : Board::RankEight);
  switch (m.Flag) {
  case Move::NoFlag:
    break;
  case Move::Promotion: {
    state.Bitboards[Board::Pawn] |= 1ul << m.OriginSquare;

    Board::Index piece;
    switch (m.PromotionPiece) {
    case Move::Knight:
      piece = Board::Knight;
      break;
    case Move::Bishop:
      piece = Board::Bishop;
      break;
    case Move::Rook:
      piece = Board::Rook;
      break;
    case Move::Queen:
      piece = Board::Queen;
      break;
    }

    state.Bitboards[piece] &= ~(1ul << m.DestinationSquare);

    break;
  }
  case Move::Castle: {
    bool isShortCastle = (m.DestinationSquare - m.OriginSquare) == (Move::Right * 2);
    int originSquare = std::__countr_zero(friendlyCastlingRank & (isShortCastle ? Board::FileA : Board::FileH));
    int destinationSquare = originSquare + (Move::Right * 2 * (isShortCastle ? 1 : -1));

    *friendlyPieces &= ~(1ul << destinationSquare);
    state.Bitboards[Board::Rook] &= ~(1ul << destinationSquare);
    *friendlyPieces |= 1ul << originSquare;
    state.Bitboards[Board::Rook] |= 1ul << originSquare;

    break;
  }
  case Move::EnPassant: {
    int captureSquare = m.DestinationSquare + (state.WhiteToPlay ? Move::Down : Move::Up);
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
