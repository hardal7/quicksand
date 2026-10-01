#include "../../include/types.h"
#include <cctype>
#include <string>

class Piece {
public:
  int type = None;
  int color = White;

  enum Bits {
    White = 0,
    None = -1,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    Black = 8
  };
};

void fenToBitboards(GameState &state, std ::array<Piece, BOARD_SQUARES> board) {
  int square = 0;
  for (Piece piece : board) {
    if (piece.type != Piece::None) {
      if (piece.color == Piece::Black) {
        state.bitboards[Board::Black] += 1ul << square;
      } else {
        state.bitboards[Board::White] += 1ul << square;
      }
      state.bitboards[piece.type] += 1ul << square;
    }
    square++;
  }
}

void loadFEN(GameState &state, std::string fen) {
  std ::array<Piece, BOARD_SQUARES> board = {Piece::None};

  int square = 0;
  for (char c : fen) {
    Piece piece = Piece();

    char cLower = std::tolower(c);
    if (c == cLower) {
      piece.color = Piece::Black;
    } else {
      piece.color = Piece::White;
    }

    switch (cLower) {
    case 'p':
      piece.type = Piece::Pawn;
      break;
    case 'n':
      piece.type = Piece::Knight;
      break;
    case 'b':
      piece.type = Piece::Bishop;
      break;
    case 'r':
      piece.type = Piece::Rook;
      break;
    case 'q':
      piece.type = Piece::Queen;
      break;
    case 'k':
      piece.type = Piece::King;
      break;

    case '1':
      break;
    case '2':
      square += 1;
      break;
    case '3':
      square += 2;
      break;
    case '4':
      square += 3;
      break;
    case '5':
      square += 4;
      break;
    case '6':
      square += 5;
      break;
    case '7':
      square += 6;
      break;
    case '8':
      square += 7;
      break;

    case '/':
      continue;
    }

    board[square] = piece;
    square += 1;
  }

  fenToBitboards(state, board);
}
