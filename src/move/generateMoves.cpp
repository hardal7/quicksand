#include "../../include/enums.h"
#include "../../include/types.h"
#include "createMoves.h"
#include <cstdint>
#include <optional>

bool occupiedByFriendly(const GameState &state, int destinationSquare) {
  Board::Bitboard friendlyPieces = state.whiteToPlay
                                       ? state.bitboards[Board::Indexes::White]
                                       : state.bitboards[Board::Indexes::Black];
  return friendlyPieces & (1ul << destinationSquare);
}
bool occupiedByOpponent(const GameState &state, int destinationSquare) {
  Board::Bitboard opponentPieces = state.whiteToPlay
                                       ? state.bitboards[Board::Indexes::Black]
                                       : state.bitboards[Board::Indexes::White];
  return opponentPieces & (1ul << destinationSquare);
}
bool destinationIsEmpty(const GameState &state, int destinationSquare) {
  Board::Bitboard allPieces = state.bitboards[Board::Indexes::White] |
                              state.bitboards[Board::Indexes::Black];
  return !(allPieces & 1ul << destinationSquare);
}
bool moveConstrained(int square, int move) {
  Board::Bitboard constraintMask = constraints.at(move);
  return constraintMask & (1ul << square);
}

void generatePawnMoves(const GameState &state, int square) {
  Board::Bitboard allPieces = state.bitboards[Board::Indexes::White] |
                              state.bitboards[Board::Indexes::Black];
  Board::Bitboard opponentPieces = state.whiteToPlay
                                       ? state.bitboards[Board::Indexes::Black]
                                       : state.bitboards[Board::Indexes::White];

  int direction = state.whiteToPlay ? Forward : Backward;
  int destinationSquare = square + Down * direction;
  if (destinationIsEmpty(state, destinationSquare)) {
    bool isPromotion =
        1ul << square & (state.whiteToPlay ? rankSevenMask : rankTwoMask);
    if (isPromotion) {
      for (Board::Piece pieceType = Board::Indexes::Knight;
           pieceType <= Board::Indexes::Queen; pieceType++) {
        createMove(square, square + Down * direction, pieceType);
      }
    } else {
      createMove(square, square + Down * direction);
    }

    destinationSquare = square + Down * 2 * direction;
    bool pawnNotMoved =
        1ul << square & (state.whiteToPlay ? rankTwoMask : rankSevenMask);
    if (destinationIsEmpty(state, destinationSquare) && pawnNotMoved) {
      createMove(square, destinationSquare);
    }
  }

  const std::array<int, 2> attackMoves = {Down + Left, Down + Right};
  for (int move : attackMoves) {
    destinationSquare = square + move * direction;
    bool occupiedByOpponent =
        (opponentPieces | (1ul << state.enPassantSquare)) &
        1ul << destinationSquare;

    uint64_t constraintMask = constraints.at(move);
    bool moveConstrained = constraintMask & (1ul << square);

    if (occupiedByOpponent && !moveConstrained) {
      createMove(square, destinationSquare);
    }
  }
}

void generateKnightMoves(const GameState &state, int square) {
  const std::array<int, 8> moves = {
      Up * 2 + Left, Up * 2 + Right, Down * 2 + Left, Down * 2 + Right,
      Up + Left * 2, Up + Right * 2, Down + Left * 2, Down + Right * 2,
  };

  for (int move : moves) {
    if (!occupiedByFriendly(state, square + move) &&
        !moveConstrained(square, move)) {
      createMove(square, square + move);
    }
  }
}

void generateBishopMoves(const GameState &state, int square) {
  const std::array<int, 4> moves = {Up + Left, Up + Right, Down + Left,
                                    Down + Right};

  for (int move : moves) {
    for (int magnitude = 1; magnitude++;) {
      int destinationSquare = (square + move * magnitude);
      if (destinationSquare < 0 || destinationSquare > BOARD_SQUARES - 1) {
        break;
      }
      if (!moveConstrained(square, move)) {
        if (occupiedByOpponent(state, destinationSquare)) {
          createMove(square, destinationSquare);
          break;
        } else if (!occupiedByFriendly(state, destinationSquare)) {
          createMove(square, destinationSquare);
        }
      }
    }
  }
}

void generateRookMoves(const GameState &state, int square) {
  const std::array<int, 4> moves = {Up, Down, Left, Right};

  for (int move : moves) {
    for (int magnitude = 1; magnitude++;) {
      int destinationSquare = (square + move * magnitude);

      if (destinationSquare < 0 || destinationSquare > BOARD_SQUARES - 1) {
        break;
      }
      if (!moveConstrained(square, move)) {
        if (occupiedByOpponent(state, destinationSquare)) {
          createMove(square, destinationSquare);
          break;
        } else if (!occupiedByFriendly(state, destinationSquare)) {
          createMove(square, destinationSquare);
        }
      }
    }
  }
}

void generateQueenMoves(const GameState &state, int square) {
  generateBishopMoves(state, square);
  generateRookMoves(state, square);
}

void generateKingMoves(const GameState &state, int square) {
  const std::array<int, 8> moves = {
      Up, Down, Left, Right, Up + Left, Up + Right, Down + Left, Down + Right};

  for (int move : moves) {
    if (!occupiedByFriendly(state, square + move) &&
        !moveConstrained(square, move)) {
      createMove(square, square + move);
    }
  }

  Board::Bitboard friendlyRank =
      (state.whiteToPlay ? rankOneMask : rankEightMask);
  Board::Bitboard longCastleMask =
      fileBMask | fileCMask | fileDMask | friendlyRank;
  Board::Bitboard shortCastleMask = fileFMask | fileGMask | friendlyRank;
  Board::Bitboard longCastleRookPosition = fileAMask & friendlyRank;
  Board::Bitboard shortCastleRookPosition = fileHMask & friendlyRank;
  Board::Bitboard castleKingSquare =
      fileE + (state.whiteToPlay ? rankOne : rankEight);
  Board::Bitboard allPieces = state.bitboards[Board::Indexes::White] |
                              state.bitboards[Board::Indexes::Black];
  Board::Bitboard friendlyRooks =
      state.bitboards[Board::Indexes::Rook] |
      state.bitboards[state.whiteToPlay ? Board::Indexes::White
                                        : Board::Indexes::Black];

  const int CASTLE_TYPES = 2;
  for (int i = 0; i < CASTLE_TYPES; i++) {
    bool castleFilesEmpty =
        !(allPieces & (i ? shortCastleMask : longCastleMask));
    bool castleRookExists =
        friendlyRooks & (i ? shortCastleRookPosition : longCastleRookPosition);
    if (square == castleKingSquare && castleFilesEmpty && castleRookExists &&
        (i ? state.canShortCastle : state.canLongCastle)) {
      createMove(square, castleKingSquare, std::nullopt, true);
    }
  }
}

void generateMoves(const GameState &state) {
  Board::Bitboard friendlyPieces =
      state.bitboards[state.whiteToPlay ? Board::Indexes::White
                                        : Board::Indexes::Black];
  for (Board::Piece pieceType = Board::Indexes::Pawn;
       pieceType <= Board::Indexes::King; pieceType++) {
    Board::Bitboard currentPiece = state.bitboards[pieceType] & friendlyPieces;
    while (currentPiece != 0) {
      int square = std::__countr_zero(currentPiece);

      switch (pieceType) {
      case Board::Indexes::Pawn:
        generatePawnMoves(state, square);
        break;
      case Board::Indexes::Knight:
        generateKnightMoves(state, square);
        break;
      case Board::Indexes::Bishop:
        generateBishopMoves(state, square);
        break;
      case Board::Indexes::Rook:
        generateRookMoves(state, square);
        break;
      case Board::Indexes::Queen:
        generateQueenMoves(state, square);
        break;
      case Board::Indexes::King:
        generateKingMoves(state, square);
        break;
      }
      currentPiece ^= (1ul << square);
    }
  }
}
