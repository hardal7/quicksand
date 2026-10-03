#include "../board/board.h"
#include "castle.h"
#include "create.h"
#include <cstdint>
#include <cstdlib>
#include <optional>

bool occupiedByFriendly(const GameState &state, int destinationSquare) {
  Board::Bitboard friendlyPieces = state.whiteToPlay
                                       ? state.bitboards[Board::White]
                                       : state.bitboards[Board::Black];
  return friendlyPieces & (1ul << destinationSquare);
}
bool occupiedByOpponent(const GameState &state, int destinationSquare) {
  Board::Bitboard opponentPieces = state.whiteToPlay
                                       ? state.bitboards[Board::Black]
                                       : state.bitboards[Board::White];
  return opponentPieces & (1ul << destinationSquare);
}
bool destinationIsEmpty(const GameState &state, int destinationSquare) {
  Board::Bitboard allPieces =
      state.bitboards[Board::White] | state.bitboards[Board::Black];
  return !(allPieces & 1ul << destinationSquare);
}
bool moveConstrained(int square, int move) {
  Board::Bitboard constraintMask = Constraints.at(move);
  return constraintMask & (1ul << square);
}

void generatePawnMoves(const GameState &state,
                       std::array<move, MAX_MOVES> &movesList, int square) {
  Board::Bitboard opponentPieces = state.whiteToPlay
                                       ? state.bitboards[Board::Black]
                                       : state.bitboards[Board::White];

  int direction = state.whiteToPlay ? Forward : Backward;
  int destinationSquare = square + Down * direction;
  if (destinationIsEmpty(state, destinationSquare)) {
    bool isPromotion =
        1ul << square & (state.whiteToPlay ? rankSevenMask : rankTwoMask);
    if (isPromotion) {
      for (Board::Piece pieceType = Board::Knight; pieceType <= Board::Queen;
           pieceType++) {
        createMove(square, square + Down * direction, movesList, pieceType);
      }
    } else {
      createMove(square, square + Down * direction, movesList);
    }

    destinationSquare = square + Down * 2 * direction;
    bool pawnNotMoved =
        1ul << square & (state.whiteToPlay ? rankTwoMask : rankSevenMask);
    if (destinationIsEmpty(state, destinationSquare) && pawnNotMoved) {
      createMove(square, destinationSquare, movesList);
    }
  }

  const std::array<int, 2> attackMoves = {Down + Left, Down + Right};
  for (int move : attackMoves) {
    destinationSquare = square + move * direction;
    bool occupiedByOpponent =
        (opponentPieces | (1ul << state.enPassantSquare)) &
        1ul << destinationSquare;

    uint64_t constraintMask = Constraints.at(move);
    bool moveConstrained = constraintMask & (1ul << square);

    if (occupiedByOpponent && !moveConstrained) {
      createMove(square, destinationSquare, movesList);
    }
  }
}

void generateKnightMoves(const GameState &state,
                         std::array<move, MAX_MOVES> &movesList, int square) {
  const std::array<int, 8> moves = {
      Up * 2 + Left, Up * 2 + Right, Down * 2 + Left, Down * 2 + Right,
      Up + Left * 2, Up + Right * 2, Down + Left * 2, Down + Right * 2,
  };

  for (int move : moves) {
    if (!occupiedByFriendly(state, square + move) &&
        !moveConstrained(square, move)) {
      createMove(square, square + move, movesList);
    }
  }
}

void generateBishopMoves(const GameState &state,
                         std::array<move, MAX_MOVES> &movesList, int square) {
  const std::array<int, 4> moves = {Up + Left, Up + Right, Down + Left,
                                    Down + Right};

  int currentRank = RANK_SQUARES - (square / RANK_SQUARES);
  int currentFile = square % RANK_SQUARES;

  for (int move : moves) {
    for (int magnitude = 1; magnitude < RANK_SQUARES; magnitude++) {
      int destinationSquare = (square + move * magnitude);
      int rank = RANK_SQUARES - (destinationSquare / RANK_SQUARES);
      int file = destinationSquare % RANK_SQUARES;

      if (destinationSquare < 0 || destinationSquare > BOARD_SQUARES - 1) {
        break;
      }
      bool isDiagonalMove =
          std::abs(currentRank - rank) == std::abs(currentFile - file);

      if (!isDiagonalMove) {
        break;
      }

      if (occupiedByOpponent(state, destinationSquare)) {
        createMove(square, destinationSquare, movesList);
        break;
      } else if (occupiedByFriendly(state, destinationSquare)) {
        break;
      } else {
        createMove(square, destinationSquare, movesList);
      }
    }
  }
}

void generateRookMoves(const GameState &state,
                       std::array<move, MAX_MOVES> &movesList, int square) {
  const std::array<int, 4> moves = {Up, Down, Left, Right};

  int currentRank = RANK_SQUARES - (square / RANK_SQUARES);
  int currentFile = square % RANK_SQUARES;

  for (int move : moves) {
    for (int magnitude = 1; magnitude < RANK_SQUARES; magnitude++) {
      int destinationSquare = (square + move * magnitude);
      int rank = RANK_SQUARES - (destinationSquare / RANK_SQUARES);
      int file = destinationSquare % RANK_SQUARES;

      if (destinationSquare < 0 || destinationSquare > BOARD_SQUARES - 1) {
        break;
      }
      bool isStraightMove = (currentRank == rank) || (currentFile == file);
      if (!isStraightMove) {
        break;
      }

      if (occupiedByOpponent(state, destinationSquare)) {
        createMove(square, destinationSquare, movesList);
        break;
      } else if (occupiedByFriendly(state, destinationSquare)) {
        break;
      } else {
        createMove(square, destinationSquare, movesList);
      }
    }
  }
}

void generateQueenMoves(const GameState &state,
                        std::array<move, MAX_MOVES> &movesList, int square) {
  generateBishopMoves(state, movesList, square);
  generateRookMoves(state, movesList, square);
}

void generateKingMoves(const GameState &state,
                       std::array<move, MAX_MOVES> &movesList, int square) {
  const std::array<int, 8> moves = {
      Up, Down, Left, Right, Up + Left, Up + Right, Down + Left, Down + Right};

  for (int move : moves) {
    if (!occupiedByFriendly(state, square + move) &&
        !moveConstrained(square, move)) {
      createMove(square, square + move, movesList);
    }
  }

  Board::Bitboard allPieces =
      state.bitboards[Board::White] | state.bitboards[Board::Black];
  Board::Bitboard friendlyRooks =
      state.bitboards[Board::Rook] |
      state.bitboards[state.whiteToPlay ? Board::White : Board::Black];

  const int CASTLE_TYPES = 2;
  for (int i = 0; i < CASTLE_TYPES; i++) {
    bool castleFilesEmpty =
        !(allPieces & (i ? shortCastleMask(state.whiteToPlay)
                         : longCastleMask(state.whiteToPlay)));
    bool castleRookExists =
        friendlyRooks & (i ? shortCastleRookPosition(state.whiteToPlay)
                           : longCastleRookPosition(state.whiteToPlay));
    bool validCastlePosition = square == unmovedKingSquare(state.whiteToPlay) &&
                               castleFilesEmpty && castleRookExists &&
                               (i ? state.canShortCastle : state.canLongCastle);
    if (validCastlePosition) {
      int destinationSquare = i ? shortcastleKingSquare(state.whiteToPlay)
                                : longcastleKingSquare(state.whiteToPlay);
      createMove(square, destinationSquare, movesList, std::nullopt, true);
    }
  }
}

void generateMoves(const GameState &state,
                   std::array<move, MAX_MOVES> &movesList) {
  Board::Bitboard friendlyPieces =
      state.bitboards[state.whiteToPlay ? Board::White : Board::Black];
  for (Board::Piece pieceType = Board::Pawn; pieceType <= Board::King;
       pieceType++) {
    Board::Bitboard board = state.bitboards[pieceType] & friendlyPieces;
    while (board) {
      int square = std::__countr_zero(board);

      switch (pieceType) {
      case Board::Pawn:
        generatePawnMoves(state, movesList, square);
        break;
      case Board::Knight:
        generateKnightMoves(state, movesList, square);
        break;
      case Board::Bishop:
        generateBishopMoves(state, movesList, square);
        break;
      case Board::Rook:
        generateRookMoves(state, movesList, square);
        break;
      case Board::Queen:
        generateQueenMoves(state, movesList, square);
        break;
      case Board::King:
        generateKingMoves(state, movesList, square);
        break;
      }
      board ^= (1ul << square);
    }
  }
}
