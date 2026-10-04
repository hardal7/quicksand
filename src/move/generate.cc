#include "../board/board.h"
#include "../types.h"
#include "move.h"

void pawnMoves(const GameState &state, Move::List &movesList) {}
void knightMoves(const GameState &state, Move::List &movesList) {}
void bishopMoves(const GameState &state, Move::List &movesList) {}
void rookMoves(const GameState &state, Move::List &movesList) {}

void queenMoves(const GameState &state, Move::List &movesList) {
  rookMoves(state, movesList);
  bishopMoves(state, movesList);
}

void kingMoves(const GameState &state, Move::List &movesList) {}

void generateMoves(const GameState &state, Move::List &movesList) {
  movesList = {Move::NoMove};

  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];

  for (int square = 0; square < Board::TotalSquares; square++) {
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (state.Bitboards[piece] & *friendlyPieces & (1ul << square)) {
        switch (piece) {
        case Board::Pawn:
          pawnMoves(state, movesList);
          break;
        }
      }
    }
  }
}
