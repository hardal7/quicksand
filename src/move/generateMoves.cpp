#include "../../include/enums.h"
#include "../../include/types.h"
#include <map>

void createMove(int currentSquare, int destinationSquare,
                int promotionPiece = Bitboard::Indexes::None) {
  if (promotionPiece != Bitboard::Indexes::None) {
  }
}

void generatePawnMoves(const GameState &state, int square) {
  uint64_t allPiecesBitboard = state.bitboards[Bitboard::Indexes::White] |
                               state.bitboards[Bitboard::Indexes::Black];
  uint64_t opponentBitboard = state.whiteToPlay
                                  ? state.bitboards[Bitboard::Indexes::Black]
                                  : state.bitboards[Bitboard::Indexes::White];

  int direction = state.whiteToPlay ? Forward : Backward;
  int destinationSquare = square + Down * direction;
  bool oneAheadEmpty = !(allPiecesBitboard & 1ul << destinationSquare);
  if (oneAheadEmpty) {
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
    bool twoAheadEmpty = !(allPiecesBitboard & 1ul << destinationSquare);
    bool pawnNotMoved =
        1ul << square & (state.whiteToPlay ? rankTwoMask : rankSevenMask);
    if (twoAheadEmpty && pawnNotMoved) {
      createMove(square, destinationSquare);
    }
  }

  const std::array<int, 2> attackMoves = {Down + Left, Down + Right};
  for (int move : attackMoves) {
    destinationSquare = square + move * direction;
    bool enemyOnAttackSquare =
        (opponentBitboard | (1ul << state.enPassantSquare)) &
        1ul << destinationSquare;

    const std::map<int, uint64_t> constraints = {{Up + Left, fileAMask},
                                                 {Down + Left, fileAMask},
                                                 {Up + Right, fileHMask},
                                                 {Down + Right, fileHMask}};
    uint64_t constraintMask = constraints.at(move);
    bool moveConstrained = constraintMask & (1ul << square);

    if (enemyOnAttackSquare && !moveConstrained) {
      createMove(square, destinationSquare);
    }
  }
}

void generateKnightMoves(const GameState &state, int square) {
  uint64_t friendlyBitboard = state.whiteToPlay
                                  ? state.bitboards[Bitboard::Indexes::White]
                                  : state.bitboards[Bitboard::Indexes::Black];

  const std::array<int, 8> moves = {
      Up * 2 + Left, Up * 2 + Right, Down * 2 - Left, Down * 2 + Right,
      Up + Left * 2, Up + Right * 2, Down + Left * 2, Down + Right * 2,
  };

  const std::map<int, uint64_t> constraints = {
      {moves[0], rankSevenMask | rankEightMask | fileAMask},
      {moves[1], rankSevenMask | rankEightMask | fileHMask},
      {moves[2], rankOneMask | rankTwoMask | fileAMask},
      {moves[3], rankOneMask | rankTwoMask | fileHMask},

      {moves[4], fileAMask | fileBMask | rankEightMask},
      {moves[5], fileAMask | fileBMask | rankOneMask},
      {moves[6], fileGMask | fileHMask | rankEightMask},
      {moves[7], fileGMask | fileHMask | rankOneMask},
  };

  for (int move : moves) {
    bool occupiedByFriendly = friendlyBitboard & (1ul << (square + move));
    uint64_t constraintMask = constraints.at(move);
    bool moveConstrained = constraintMask & (1ul << square);
    if (!occupiedByFriendly && !moveConstrained) {
      createMove(square, square + move);
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
      default:
        break;
      }
      currentPieceBitboard -= (1ul << square);
    }
  }
}
