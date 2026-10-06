#include "eval.h"
#include "../types.h"
#include "tables.h"

const int EndgameCutoff = 15;

int evaluatePosition(const GameState &state) {
  int eval = 0;

  int allPieces = __builtin_popcountll(state.Bitboards[Board::White] | state.Bitboards[Board::Black]);
  bool endgame = allPieces <= EndgameCutoff;

  for (int color = 0; color < Board::TotalColors; color++) {
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      Board::bitboard board = state.Bitboards[piece] & state.Bitboards[color ? Board::White : Board::Black];

      int totalPieces = __builtin_popcountll(board);
      eval += pieceValues.at(piece) * totalPieces * (color ? 1 : -1);

      while (board) {
        int square = std::__countr_zero(board);
        eval += (color ? (endgame ? whiteEndgameTables : whiteTables)
                       : (endgame ? blackEndgameTables : blackTables))[piece][square] *
                (color ? 1 : -1);
        board &= ~(1ul << square);
      }
    }
  }

  return eval;
}
