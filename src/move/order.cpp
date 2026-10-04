#include "order.h"
#include "encode.h"
#include "enums.h"
#include "make.h"
#include <algorithm>
#include <array>

void orderMoves(GameState &state, std::array<ScoredMove, MAX_MOVES> &moves) {
  for (ScoredMove &m : moves) {
    if (m.value == NO_MOVE) {
      break;
    }

    int movedPiece = Board::None;
    int originSquare = (m.value & OriginSquareOffsetMask) >> OriginSquareOffset;
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if ((1ul << originSquare) & piece) {
        movedPiece = piece;
      }
    }

    Board::Piece capturedPiece = makeMove(state, m.value);

    if (capturedPiece != Board::None) {
      m.score += capturedPiece * 100 - movedPiece;
    }

    unmakeMove(state, m.value, capturedPiece);
  }

  std::sort(moves.begin(), moves.end(),
            [](const ScoredMove &a, const ScoredMove &b) {
              if ((a.value == 0) != (b.value == 0))
                return a.value != 0;

              return a.score > b.score;
            });
}
