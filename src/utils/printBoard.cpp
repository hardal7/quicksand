#include "../../include/types.h"
#include <cctype>
#include <iostream>
#include <map>
#include <string>

const std::map<int, char> pieceChars = {
    {Board::Indexes::Pawn, 'p'},   {Board::Indexes::Knight, 'n'},
    {Board::Indexes::Bishop, 'b'}, {Board::Indexes::Rook, 'r'},
    {Board::Indexes::Queen, 'q'},  {Board::Indexes::King, 'k'},
};

void printBoard(GameState state) {
  std::string board = "";

  for (int square = 0; square < BOARD_SQUARES; square++) {
    if (square % RANK_SQUARES == 0) {
      board += "\n";
    }

    bool foundPiece = false;
    for (int piece = Board::Indexes::Pawn; piece <= Board::Indexes::King;
         piece++) {
      if (state.bitboards[piece] & (1ul << square)) {
        char pieceChar = pieceChars.at(piece);
        if (state.bitboards[Board::Indexes::White] & (1ul << square)) {
          pieceChar = std::toupper(pieceChar);
        }

        board += pieceChar;
        board += " ";
        foundPiece = true;
        break;
      }
    }

    if (!foundPiece) {
      board += ". ";
    }
  }

  std::cout << board << std::endl;
}
