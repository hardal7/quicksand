#include "../board/board.h"
#include "../types.h"
#include "move.h"

using namespace Move;

const Board::bitboard *friendlyPieces(const GameState &state) {
  return &state.Bitboards[state.WhiteToPlay ? Board::White : Board::Black];
}
const Board::bitboard *opponentPieces(const GameState &state) {
  return &state.Bitboards[state.WhiteToPlay ? Board::Black : Board::White];
}

void pawnMoves(const GameState &state, int square, List &movesList) {
  const Board::bitboard allPieces = *friendlyPieces(state) | *opponentPieces(state);
  int direction = state.WhiteToPlay ? 1 : -1;

  bool isPromoting = (1ul << square) & (state.WhiteToPlay ? Board::RankSeven : Board::RankTwo);
  Flag promotionFlag = NoFlag;
  if (isPromoting) {
    promotionFlag = Promotion;
  }

  int destinationSquare = square + Up * direction;
  bool destinationOccupied = allPieces & (1ul << destinationSquare);
  if (!destinationOccupied) {
    insert(movesList, Readable{square, destinationSquare, promotionFlag});

    bool notMoved = (1ul << square) & (state.WhiteToPlay ? Board::RankTwo : Board::RankSeven);
    int destinationSquare = square + Up * 2 * direction;
    bool destinationOccupied = allPieces & (1ul << destinationSquare);
    if (!destinationOccupied && notMoved) {
      insert(movesList, Readable{square, destinationSquare, promotionFlag});
    }
  }

  const std::array<int, 2> captureMoves = {Up + Left, Up + Right};
  for (auto move : captureMoves) {
    int destinationSquare = square + move * direction;
    bool opponentOnDestination = (*opponentPieces(state) | (1ul << state.enPassantSquare)) & (1ul << destinationSquare);
    if (opponentOnDestination) {
      if (destinationSquare == state.enPassantSquare) {
        insert(movesList, Readable{square, destinationSquare, EnPassant});
      } else {
        insert(movesList, Readable{square, destinationSquare, promotionFlag});
      }
    }
  }
}

void knightMoves(const GameState &state, int square, List &movesList) {
  // clang-format off
  const std::array<int, 8> knightMoves = {
      Up * 2 + Left, Up * 2 + Right,
      Down * 2 + Left, Down * 2 + Right,
      Left * 2 + Up, Left * 2 + Down,
      Right * 2 + Up,  Right * 2 + Down,
  };
  // clang-format on

  for (auto move : knightMoves) {
    int destinationSquare = square + move;
    bool friendlyOnDestination = *friendlyPieces(state) & (1ul << destinationSquare);
    if (!friendlyOnDestination) {
      insert(movesList, Readable{square, destinationSquare});
    }
  }
}

void bishopMoves(const GameState &state, int square, List &movesList) {
  const std::array<int, 4> bishopMoves = {Up + Right, Up + Left, Down + Left, Down + Right};

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
        insert(movesList, Readable{square, destinationSquare});
        break;
      } else {
        insert(movesList, Readable{square, destinationSquare});
      }
    }
  }
}

void rookMoves(const GameState &state, int square, List &movesList) {
  const std::array<int, 4> rookMoves = {Up, Down, Left, Right};

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
        insert(movesList, Readable{square, destinationSquare});
        break;
      } else {
        insert(movesList, Readable{square, destinationSquare});
      }
    }
  }
}

void queenMoves(const GameState &state, int square, List &movesList) {
  rookMoves(state, square, movesList);
  bishopMoves(state, square, movesList);
}

void kingMoves(const GameState &state, int square, List &movesList) {
  const std::array<int, 8> kingMoves = {
      Up,        Down,       Left,        Right,

      Up + Left, Up + Right, Down + Left, Down + Right,
  };

  for (auto move : kingMoves) {
    int destinationSquare = square + move;
    bool friendlyOnDestination = *friendlyPieces(state) & (1ul << destinationSquare);
    if (!friendlyOnDestination) {
      insert(movesList, Readable{square, destinationSquare});
    }
  }

  Board::bitboard allPieces = *friendlyPieces(state) | *opponentPieces(state);
  Board::bitboard friendlyCastlingRank = (state.WhiteToPlay ? Board::RankOne : Board::RankEight);
  Board::bitboard friendlyRooks = (*friendlyPieces(state) & state.Bitboards[Board::Rook]);
  const std::array<int, 2> kingCastleMoves = {Left * 3, Right * 2};

  bool kingNotMoved = (1ul << square) & (Board::FileE & friendlyCastlingRank);
  bool shortRookNotMoved = friendlyRooks & (Board::FileH & friendlyCastlingRank);
  bool longRookNotMoved = friendlyRooks & (Board::FileA & friendlyCastlingRank);
  bool shortCastleObstructed = allPieces & (friendlyCastlingRank & (Board::FileF | Board::FileG));
  bool longCastleObstructed = allPieces & (friendlyCastlingRank & (Board::FileB | Board::FileC | Board::FileD));

  for (int type = 0; type < 2; type++) {
    int destinationSquare = square + kingCastleMoves[type];

    bool validCastleState = kingNotMoved && (type ? shortRookNotMoved : longRookNotMoved);
    if (validCastleState && !(type ? shortCastleObstructed : longCastleObstructed)) {
      insert(movesList, Readable{square, destinationSquare, Castle});
    }
  }
}

List Move::generate(const GameState &state) {
  List movesList;

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
