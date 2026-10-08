#include "order.h"
#include "../board/board.h"
#include "../eval/eval.h"
#include "../move/move.h"
#include "../game.h"

OrderedList orderMoves(const GameState &state, Move::List moves) {
  OrderedList movesOrdered;

  for (int i = 0; i < moves.length; i++) {
    Move::Readable m = Move::decode(moves.list[i]);

    Board::Piece capturedPiece = Board::None;
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (state.Bitboards[piece] & (1ul << m.DestinationSquare)) {
        capturedPiece = piece;
        break;
      }
    }

    Board::Piece movedPiece = Board::None;
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (state.Bitboards[piece] & (1ul << m.OriginSquare)) {
        movedPiece = piece;
        break;
      }
    }

    int index = movesOrdered.length;
    if (capturedPiece != Board::None) {
      movesOrdered.list[index].Score = pieceValues.at(capturedPiece) - (pieceValues.at(movedPiece) * 0.1);
    }

    movesOrdered.list[index].Move = moves.list[i];
    movesOrdered.length++;
  }

  std::sort(movesOrdered.list.begin(), movesOrdered.list.begin() + movesOrdered.length,
            [](const ScoredMove &a, const ScoredMove &b) { return a.Score > b.Score; });

  return movesOrdered;
}
