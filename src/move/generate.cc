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

void knightMoves(const GameState &state, int square, Move::List &movesList) {
  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];

  // clang-format off
  const std::array<int, 8> knightMoves = {
      Board::Up * 2 + Board::Left, Board::Up * 2 + Board::Right,
      Board::Down * 2 + Board::Left, Board::Down * 2 + Board::Right,
      Board::Left * 2 + Board::Up, Board::Left * 2 + Board::Down,
      Board::Right * 2 + Board::Up,  Board::Right * 2 + Board::Down,
  };
  // clang-format on

  for (auto move : knightMoves) {
    int destinationSquare = square + move;
    bool friendlyOnDestination = *friendlyPieces & (1ul << destinationSquare);
    if (!friendlyOnDestination) {
      Move::insert(movesList, Move::Readable{square, destinationSquare});
    }
  }
}

void bishopMoves(const GameState &state, int square, Move::List &movesList) {
  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  const Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];
  const std::array<int, 4> bishopMoves = {Board::Up + Board::Right, Board::Up + Board::Left, Board::Down + Board::Left,
                                          Board::Down + Board::Right};

  for (auto move : bishopMoves) {
    for (int magnitude = 1; magnitude <= Board::RankSquares; magnitude++) {
      int destinationSquare = square + move * magnitude;
      if (destinationSquare < 0 || destinationSquare > Board::TotalSquares - 1) {
        break;
      }

      bool friendlyOnDestination = *friendlyPieces & (1ul << destinationSquare);
      bool opponentOnDestination = *opponentPieces & (1ul << destinationSquare);

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
  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
  const Board::bitboard *opponentPieces = &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];
  const std::array<int, 4> rookMoves = {Board::Up, Board::Down, Board::Left, Board::Right};

  for (auto move : rookMoves) {
    for (int magnitude = 1; magnitude <= Board::RankSquares; magnitude++) {
      int destinationSquare = square + move * magnitude;
      if (destinationSquare < 0 || destinationSquare > Board::TotalSquares - 1) {
        break;
      }

      bool friendlyOnDestination = *friendlyPieces & (1ul << destinationSquare);
      bool opponentOnDestination = *opponentPieces & (1ul << destinationSquare);

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
  const Board::bitboard *friendlyPieces = &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];

  const std::array<int, 8> kingMoves = {
      Board::Up,
      Board::Down,
      Board::Left,
      Board::Right,

      Board::Up + Board::Left,
      Board::Up + Board::Right,
      Board::Down + Board::Left,
      Board::Down + Board::Right,
  };

  for (auto move : kingMoves) {
    int destinationSquare = square + move;
    bool friendlyOnDestination = *friendlyPieces & (1ul << destinationSquare);
    if (!friendlyOnDestination) {
      Move::insert(movesList, Move::Readable{square, destinationSquare});
    }
  }
}

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
}
