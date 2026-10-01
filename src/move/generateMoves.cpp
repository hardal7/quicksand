#include "../../include/enums.h"
#include "../../include/types.h"
#include "createMove.h"
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

void generatePawnMoves(const GameState &state,
                       std::array<move, MAX_MOVES> &movesList, int square) {
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

    uint64_t constraintMask = constraints.at(move);
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

  for (int move : moves) {
    for (int magnitude = 1; magnitude++;) {
      int destinationSquare = (square + move * magnitude);
      if (destinationSquare < 0 || destinationSquare > BOARD_SQUARES - 1) {
        break;
      }
      if (!moveConstrained(square, move)) {
        if (occupiedByOpponent(state, destinationSquare)) {
          createMove(square, destinationSquare, movesList);
          break;
        } else if (!occupiedByFriendly(state, destinationSquare)) {
          break;
        }
      }
    }
  }
}

void generateRookMoves(const GameState &state,
                       std::array<move, MAX_MOVES> &movesList, int square) {
  const std::array<int, 4> moves = {Up, Down, Left, Right};

  for (int move : moves) {
    for (int magnitude = 1; magnitude++;) {
      int destinationSquare = (square + move * magnitude);

      if (destinationSquare < 0 || destinationSquare > BOARD_SQUARES - 1) {
        break;
      }
      if (!moveConstrained(square, move)) {
        if (occupiedByOpponent(state, destinationSquare)) {
          createMove(square, destinationSquare, movesList);
          break;
        } else if (!occupiedByFriendly(state, destinationSquare)) {
          break;
        }
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

  Board::Bitboard friendlyRank =
      (state.whiteToPlay ? rankOneMask : rankEightMask);
  Board::Bitboard longCastleMask =
      fileBMask | fileCMask | fileDMask | friendlyRank;
  Board::Bitboard shortCastleMask = fileFMask | fileGMask | friendlyRank;
  Board::Bitboard longCastleRookPosition = fileAMask & friendlyRank;
  Board::Bitboard shortCastleRookPosition = fileHMask & friendlyRank;
  int castleKingSquare = fileE + (state.whiteToPlay ? rankOne : rankEight);
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
      createMove(square, castleKingSquare, movesList, std::nullopt, true);
    }
  }
}

void generateMoves(const GameState &state,
                   std::array<move, MAX_MOVES> &movesList) {
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
        generatePawnMoves(state, movesList, square);
        break;
      case Board::Indexes::Knight:
        generateKnightMoves(state, movesList, square);
        break;
      case Board::Indexes::Bishop:
        generateBishopMoves(state, movesList, square);
        break;
      case Board::Indexes::Rook:
        generateRookMoves(state, movesList, square);
        break;
      case Board::Indexes::Queen:
        generateQueenMoves(state, movesList, square);
        break;
      case Board::Indexes::King:
        generateKingMoves(state, movesList, square);
        break;
      }
      currentPiece ^= (1ul << square);
    }
  }
}
