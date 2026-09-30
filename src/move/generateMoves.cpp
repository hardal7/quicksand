#include "../../include/enums.h"
#include "../../include/types.h"
#include <map>

void createMove(int currentSquare, int destinationSquare,
                int promotionPiece = Bitboard::Indexes::None) {
  if (promotionPiece != Bitboard::Indexes::None) {
  }
}

void generatePawnMoves(GameState state) {
  uint64_t allPiecesBitboard = state.bitboards[Bitboard::Indexes::White] |
                               state.bitboards[Bitboard::Indexes::Black];
  uint64_t opponentBitboard = state.whiteToPlay
                                  ? state.bitboards[Bitboard::Indexes::Black]
                                  : state.bitboards[Bitboard::Indexes::White];
  uint64_t currentPawnsBitboard =
      state.bitboards[Bitboard::Indexes::Pawn] &
      state.bitboards[state.whiteToPlay ? Bitboard::Indexes::White
                                        : Bitboard::Indexes::Black];

  while (currentPawnsBitboard != 0) {
    int lsb = currentPawnsBitboard & -currentPawnsBitboard;
    int square = __builtin_ctzll(lsb);

    int direction = state.whiteToPlay ? -1 : 1;

    int destinationSquare = square + RANK_SQUARES * direction;
    bool oneAheadEmpty = !(allPiecesBitboard & 1ul << destinationSquare);
    if (oneAheadEmpty) {
      bool isPromotion =
          1ul << square & (state.whiteToPlay ? rankSevenMask : rankTwoMask);
      if (isPromotion) {
        for (int pieceType = Bitboard::Indexes::Knight;
             pieceType <= Bitboard::Indexes::Queen; pieceType++) {
          createMove(square, square + RANK_SQUARES * direction, pieceType);
        }
      } else {
        createMove(square, square + RANK_SQUARES * direction);
      }

      destinationSquare = square + RANK_SQUARES * 2 * direction;
      bool twoAheadEmpty = !(allPiecesBitboard & 1ul << destinationSquare);
      bool pawnNotMoved =
          1ul << square & (state.whiteToPlay ? rankTwoMask : rankSevenMask);
      if (twoAheadEmpty && pawnNotMoved) {
        createMove(square, destinationSquare);
      }
    }

    const std::array<int, 2> attackMoves = {RANK_SQUARES - 1, RANK_SQUARES + 1};
    for (int move : attackMoves) {
      destinationSquare = square + move * direction;
      bool enemyOnAttackSquare =
          (opponentBitboard | (1ul << state.enPassantSquare)) &
          1ul << destinationSquare;

      std::map<uint64_t, int> constraints = {{fileAMask, -RANK_SQUARES - 1},
                                             {fileAMask, RANK_SQUARES - 1},
                                             {fileHMask, RANK_SQUARES + 1},
                                             {fileHMask, -RANK_SQUARES + 1}};
      uint64_t constraintMask = constraints.at(move);
      bool moveConstrained = constraintMask & (1ul << square);

      if (enemyOnAttackSquare && !moveConstrained) {
        createMove(square, destinationSquare);
      }
    }
  }
}

void generateMoves(GameState state) { generatePawnMoves(state); }
