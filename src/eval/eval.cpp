#include "tables.h"
#include <map>

const int ENDGAME_CUTOFF = 15;

int evaluateBoard(const GameState &state) {
  int eval = 0;

  const std::map<Board::Piece, int> pieceValues = {
      {Board::Pawn, 100}, {Board::Knight, 350}, {Board::Bishop, 350},
      {Board::Rook, 525}, {Board::Queen, 1000}, {Board::King, 20000},
  };

  int allPieces = __builtin_popcountll(state.bitboards[Board::White] |
                                       state.bitboards[Board::Black]);
  bool endgame = allPieces <= ENDGAME_CUTOFF;

  for (int color = 0; color < 2; color++) {
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      Board::Bitboard board =
          state.bitboards[piece] &
          state.bitboards[color ? Board::White : Board::Black];

      int totalPieces = __builtin_popcountll(board);
      eval += pieceValues.at(piece) * totalPieces * (color ? 1 : -1);

      while (board) {
        int square = std::__countr_zero(board);
        eval += (color ? (endgame ? whiteEndgameTables : whiteTables)
                       : (endgame ? blackEndgameTables
                                  : blackTables))[piece][square] *
                (color ? 1 : -1);
        board ^= (1ul << square);
      }
    }
  }

  return eval;
}
