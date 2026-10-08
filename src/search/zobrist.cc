#include "zobrist.h"
#include "../interface/polyglot.h"
#include "../move/move.h"
#include "../game.h"
#include <cstdint>

int popLSB(uint64_t &bb) {
  int square = __builtin_ctzll(bb);
  bb &= bb - 1;
  return square;
}

bool enPassantPossible(const GameState &state) {
  if (state.enPassantSquare != 0) {
    int direction = state.WhiteToPlay ? Move::Up : Move::Down;
    Board::bitboard possiblePositions = (1ul << (state.enPassantSquare + direction + Move::Left)) |
                                        (1ul << (state.enPassantSquare + direction + Move::Right));
    Board::bitboard pawns = state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
    Board::bitboard adjacentPawns = pawns & possiblePositions;

    return adjacentPawns != 0;
  }

  return false;
}

Key getZobristKey(const GameState &state) {
  Key key = 0;

  uint64_t white = state.Bitboards[Board::White];
  uint64_t black = state.Bitboards[Board::Black];

  for (Board::Piece piece = Board::Pawn; piece < Board::TotalPieces; ++piece) {
    Board::bitboard whitePieces = state.Bitboards[piece] & white;
    while (whitePieces) {
      int square = popLSB(whitePieces);

      int file = square % Board::FileSquares;
      int rank = (Board::RankSquares - 1) - (square / Board::RankSquares);

      const int WhitePieceOffset = 1;
      int polyglotPiece = piece * 2 + WhitePieceOffset;

      key ^= Random64[(Board::TotalSquares * polyglotPiece) + (Board::RankSquares * rank) + file];
    }

    Board::bitboard blackPieces = state.Bitboards[piece] & black;
    while (blackPieces) {
      int square = popLSB(blackPieces);

      int file = square % Board::FileSquares;
      int rank = (Board::RankSquares - 1) - (square / Board::RankSquares);

      int polyglotPiece = piece * 2;

      key ^= Random64[(Board::TotalSquares * polyglotPiece) + (Board::RankSquares * rank) + file];
    }
  }

  if (state.WhiteShortCastle)
    key ^= Random64[768];
  if (state.WhiteLongCastle)
    key ^= Random64[769];
  if (state.BlackShortCastle)
    key ^= Random64[770];
  if (state.BlackLongCastle)
    key ^= Random64[771];

  if (enPassantPossible(state)) {
    int file = state.enPassantSquare % Board::FileSquares;
    key ^= Random64[772 + file];
  }

  if (state.WhiteToPlay) {
    key ^= Random64[780];
  }

  return key;
}
