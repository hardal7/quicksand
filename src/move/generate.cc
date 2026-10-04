#include "../board/board.h"
#include "../types.h"
#include "move.h"

void pawnMoves(const GameState &state, int square, Move::List &movesList) {
  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  const Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];
  const Board::bitboard allPieces = *friendlyPieces | *opponentPieces;
  int direction = state.WhiteToPlay ? 1 : -1;

  bool isPromoting = std::min(square, Board::TotalSquares - square) + 1 <= (Board::RankSquares * 2);
  Move::Flag promotionFlag = Move::NoFlag;
  if (isPromoting) {
    promotionFlag = Move::Promotion;
  }

  int destinationSquare = square + Board::Up * direction;
  bool destinationOccupied = allPieces & (1ul << destinationSquare);
  if (!destinationOccupied) {
    Move::insert(movesList, Move::Readable{square, destinationSquare, promotionFlag});

    int destinationSquare = square + Board::Up * 2 * direction;
    bool destinationOccupied = allPieces & (1ul << destinationSquare);
    if (!destinationOccupied) {
      Move::insert(movesList, Move::Readable{square, destinationSquare, promotionFlag});
    }
  }

  const std::array<int, 2> captureMoves = {Board::Up + Board::Left, Board::Up + Board::Right};
  for (auto move : captureMoves) {
    int destinationSquare = square + move * direction;
    bool opponentOnDestination = *opponentPieces & state.enPassantSquare & (1ul << destinationSquare);
    if (opponentOnDestination) {
      Move::Flag enPassantFlag = Move::NoFlag;
      if (destinationSquare == state.enPassantSquare) {
        enPassantFlag = Move::EnPassant;
        Move::insert(movesList, Move::Readable{square, destinationSquare, enPassantFlag});
      } else {
        Move::insert(movesList, Move::Readable{square, destinationSquare, promotionFlag});
      }
    }
  }
}

void knightMoves(const GameState &state, int square, Move::List &movesList) {}
void bishopMoves(const GameState &state, int square, Move::List &movesList) {}
void rookMoves(const GameState &state, int square, Move::List &movesList) {}

void queenMoves(const GameState &state, int square, Move::List &movesList) {
  rookMoves(state, square, movesList);
  bishopMoves(state, square, movesList);
}

void kingMoves(const GameState &state, int square, Move::List &movesList) {}

void generateMoves(const GameState &state, Move::List &movesList) {
  movesList = {Move::NoMove};

  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];

  for (int square = 0; square < Board::TotalSquares; square++) {
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (state.Bitboards[piece] & *friendlyPieces & (1ul << square)) {
        switch (piece) {
        case Board::Pawn:
          pawnMoves(state, square, movesList);
          break;
        }
      }
    }
  }
}
