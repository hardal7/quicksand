#include "../board/board.h"
#include "../types.h"
#include "move.h"

const Board::bitboard *friendlyPieces(const GameState &state) {
  return &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
}
const Board::bitboard *opponentPieces(const GameState &state) {
  return &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];
}

void pawnMoves(const GameState &state, int square, Move::List &movesList) {
  const Board::bitboard allPieces = *friendlyPieces(state) | *opponentPieces(state);
  int direction = state.WhiteToPlay ? 1 : -1;

  bool isPromoting = (1ul << square) & (state.WhiteToPlay ? Board::RankSeven : Board::RankTwo);
  Move::Flag promotionFlag = Move::NoFlag;
  if (isPromoting) {
    promotionFlag = Move::Promotion;
  }

  int destinationSquare = square + Move::Up * direction;
  bool destinationOccupied = allPieces & (1ul << destinationSquare);
  if (!destinationOccupied) {
    Move::insert(movesList, Move::Readable{square, destinationSquare, promotionFlag});

    bool notMoved = (1ul << square) & (state.WhiteToPlay ? Board::RankTwo : Board::RankSeven);
    int destinationSquare = square + Move::Up * 2 * direction;
    bool destinationOccupied = allPieces & (1ul << destinationSquare);
    if (!destinationOccupied && notMoved) {
      Move::insert(movesList, Move::Readable{square, destinationSquare, promotionFlag});
    }
  }

  const std::array<int, 2> captureMoves = {Move::Up + Move::Left, Move::Up + Move::Right};
  for (auto move : captureMoves) {
    int destinationSquare = square + move * direction;
    bool opponentOnDestination = *opponentPieces(state) & state.enPassantSquare & (1ul << destinationSquare);
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

void knightMoves(const GameState &state, int square, Move::List &movesList) {
  // clang-format off
  const std::array<int, 8> knightMoves = {
      Move::Up * 2 + Move::Left, Move::Up * 2 + Move::Right,
      Move::Down * 2 + Move::Left, Move::Down * 2 + Move::Right,
      Move::Left * 2 + Move::Up, Move::Left * 2 + Move::Down,
      Move::Right * 2 + Move::Up,  Move::Right * 2 + Move::Down,
  };
  // clang-format on

  for (auto move : knightMoves) {
    int destinationSquare = square + move;
    bool friendlyOnDestination = *friendlyPieces(state) & (1ul << destinationSquare);
    if (!friendlyOnDestination) {
      Move::insert(movesList, Move::Readable{square, destinationSquare});
    }
  }
}

void bishopMoves(const GameState &state, int square, Move::List &movesList) {
  const std::array<int, 4> bishopMoves = {Move::Up + Move::Right, Move::Up + Move::Left, Move::Down + Move::Left,
                                          Move::Down + Move::Right};

  for (auto move : bishopMoves) {
    for (int magnitude = 1; magnitude <= Board::RankSquares; magnitude++) {
      int destinationSquare = square + move * magnitude;
      if (destinationSquare < 0 || destinationSquare > Board::TotalSquares - 1) {
        break;
      }

      int originRank = Board::RankSquares - (square / Board::RankSquares);
      int originFile = square % Board::FileSquares;
      int destinationRank = Board::RankSquares - (destinationSquare / Board::RankSquares);
      int destinationFile = destinationSquare % Board::FileSquares;
      bool isDiagonalMove = std::abs(originRank - destinationRank) == std::abs(originFile - destinationFile);
      if (!isDiagonalMove) {
        break;
      }

      bool friendlyOnDestination = *friendlyPieces(state) & (1ul << destinationSquare);
      bool opponentOnDestination = *opponentPieces(state) & (1ul << destinationSquare);

      if (friendlyOnDestination) {
        break;
      } else if (opponentOnDestination) {
        Move::insert(movesList, Move::Readable{square, destinationSquare});
        break;
      } else {
        Move::insert(movesList, Move::Readable{square, destinationSquare});
      }
    }
  }
}

void rookMoves(const GameState &state, int square, Move::List &movesList) {
  const std::array<int, 4> rookMoves = {Move::Up, Move::Down, Move::Left, Move::Right};

  for (auto move : rookMoves) {
    for (int magnitude = 1; magnitude <= Board::RankSquares; magnitude++) {
      int destinationSquare = square + move * magnitude;
      if (destinationSquare < 0 || destinationSquare > Board::TotalSquares - 1) {
        break;
      }

      int originRank = Board::RankSquares - (square / Board::RankSquares);
      int originFile = square % Board::FileSquares;
      int destinationRank = Board::RankSquares - (destinationSquare / Board::RankSquares);
      int destinationFile = destinationSquare % Board::FileSquares;
      bool isStraightMove = (originRank == destinationRank) || (originFile == destinationFile);
      if (!isStraightMove) {
        break;
      }

      bool friendlyOnDestination = *friendlyPieces(state) & (1ul << destinationSquare);
      bool opponentOnDestination = *opponentPieces(state) & (1ul << destinationSquare);

      if (friendlyOnDestination) {
        break;
      } else if (opponentOnDestination) {
        Move::insert(movesList, Move::Readable{square, destinationSquare});
        break;
      } else {
        Move::insert(movesList, Move::Readable{square, destinationSquare});
      }
    }
  }
}

void queenMoves(const GameState &state, int square, Move::List &movesList) {
  rookMoves(state, square, movesList);
  bishopMoves(state, square, movesList);
}

void kingMoves(const GameState &state, int square, Move::List &movesList) {
  const std::array<int, 8> kingMoves = {
      Move::Up,
      Move::Down,
      Move::Left,
      Move::Right,

      Move::Up + Move::Left,
      Move::Up + Move::Right,
      Move::Down + Move::Left,
      Move::Down + Move::Right,
  };

  for (auto move : kingMoves) {
    int destinationSquare = square + move;
    bool friendlyOnDestination = *friendlyPieces(state) & (1ul << destinationSquare);
    if (!friendlyOnDestination) {
      Move::insert(movesList, Move::Readable{square, destinationSquare});
    }
  }
}

Move::List generateMoves(const GameState &state) {
  Move::List movesList;

  for (int square = 0; square < Board::TotalSquares; square++) {
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (state.Bitboards[piece] & *friendlyPieces(state) & (1ul << square)) {
        switch (piece) {
        case Board::Pawn:
          pawnMoves(state, square, movesList);
          break;
        case Board::Knight:
          knightMoves(state, square, movesList);
          break;
        case Board::Bishop:
          bishopMoves(state, square, movesList);
          break;
        case Board::Rook:
          rookMoves(state, square, movesList);
          break;
        case Board::Queen:
          queenMoves(state, square, movesList);
          break;
        case Board::King:
          kingMoves(state, square, movesList);
          break;
        }
      }
    }
  }

  return movesList;
}
