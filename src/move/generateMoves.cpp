#include "../../include/enums.h"
#include "../../include/types.h"
#include <cstdint>
#include <optional>

void createMove(int currentSquare, int destinationSquare,
                std::optional<int> promotionPiece = Bitboard::Indexes::None,
                std::optional<bool> isCastle = false) {
  if (promotionPiece != Bitboard::Indexes::None) {
  } else if (isCastle) {
  }
  // TODO: Set GameState for canCastle's respectively if rook is moving.
}

bool occupiedByFriendly(const GameState &state, int destinationSquare) {
  uint64_t friendlyBitboard = state.whiteToPlay
                                  ? state.bitboards[Bitboard::Indexes::White]
                                  : state.bitboards[Bitboard::Indexes::Black];
  return friendlyBitboard & (1ul << destinationSquare);
}
bool occupiedByOpponent(const GameState &state, int destinationSquare) {
  uint64_t opponentBitboard = state.whiteToPlay
                                  ? state.bitboards[Bitboard::Indexes::Black]
                                  : state.bitboards[Bitboard::Indexes::White];
  return opponentBitboard & (1ul << destinationSquare);
}
bool destinationIsEmpty(const GameState &state, int destinationSquare) {
  uint64_t allPiecesBitboard = state.bitboards[Bitboard::Indexes::White] |
                               state.bitboards[Bitboard::Indexes::Black];
  return !(allPiecesBitboard & 1ul << destinationSquare);
}
bool moveConstrained(int square, int move) {
  uint64_t constraintMask = constraints.at(move);
  return constraintMask & (1ul << square);
}

void generatePawnMoves(const GameState &state, int square) {
  uint64_t allPiecesBitboard = state.bitboards[Bitboard::Indexes::White] |
                               state.bitboards[Bitboard::Indexes::Black];
  uint64_t opponentBitboard = state.whiteToPlay
                                  ? state.bitboards[Bitboard::Indexes::Black]
                                  : state.bitboards[Bitboard::Indexes::White];

  int direction = state.whiteToPlay ? Forward : Backward;
  int destinationSquare = square + Down * direction;
  if (destinationIsEmpty(state, destinationSquare)) {
    bool isPromotion =
        1ul << square & (state.whiteToPlay ? rankSevenMask : rankTwoMask);
    if (isPromotion) {
      for (int pieceType = Bitboard::Indexes::Knight;
           pieceType <= Bitboard::Indexes::Queen; pieceType++) {
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
        (opponentBitboard | (1ul << state.enPassantSquare)) &
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

  uint64_t friendlyRank = (state.whiteToPlay ? rankOneMask : rankEightMask);
  uint64_t longCastleMask = fileBMask | fileCMask | fileDMask | friendlyRank;
  uint64_t shortCastleMask = fileFMask | fileGMask | friendlyRank;
  uint64_t longCastleRookPosition = fileAMask & friendlyRank;
  uint64_t shortCastleRookPosition = fileHMask & friendlyRank;
  uint64_t castleKingSquare = fileE + (state.whiteToPlay ? rankOne : rankEight);
  uint64_t allPiecesBitboard = state.bitboards[Bitboard::Indexes::White] |
                               state.bitboards[Bitboard::Indexes::Black];
  uint64_t friendlyRooksBitboard =
      state.bitboards[Bitboard::Indexes::Rook] |
      state.bitboards[state.whiteToPlay ? Bitboard::Indexes::White
                                        : Bitboard::Indexes::Black];

  const int CASTLE_TYPES = 2;
  for (int i = 0; i < CASTLE_TYPES; i++) {
    bool castleFilesEmpty =
        !(allPiecesBitboard & (i ? shortCastleMask : longCastleMask));
    bool castleRookExists =
        friendlyRooksBitboard &
        (i ? shortCastleRookPosition : longCastleRookPosition);
    if (square == castleKingSquare && castleFilesEmpty && castleRookExists &&
        (i ? state.canShortCastle : state.canLongCastle)) {
      createMove(square, castleKingSquare, std::nullopt, true);
    }
  }
}

void generateMoves(const GameState &state) {
  uint64_t colorBitboard =
      state.bitboards[state.whiteToPlay ? Bitboard::Indexes::White
                                        : Bitboard::Indexes::Black];
  for (int pieceType = Bitboard::Indexes::Pawn;
       pieceType <= Bitboard::Indexes::King; pieceType++) {
    uint64_t currentPieceBitboard = state.bitboards[pieceType] & colorBitboard;
    while (currentPieceBitboard != 0) {
      int square = std::__countr_zero(currentPieceBitboard);

      switch (pieceType) {
      case Bitboard::Indexes::Pawn:
        generatePawnMoves(state, square);
        break;
      case Bitboard::Indexes::Knight:
        generateKnightMoves(state, square);
        break;
      case Bitboard::Indexes::Bishop:
        generateBishopMoves(state, square);
        break;
      case Bitboard::Indexes::Rook:
        generateRookMoves(state, square);
        break;
      case Bitboard::Indexes::Queen:
        generateQueenMoves(state, square);
        break;
      case Bitboard::Indexes::King:
        generateKingMoves(state, square);
        break;
      }
      currentPieceBitboard -= (1ul << square);
    }
  }
}
