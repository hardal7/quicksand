#include "../types.h"
#include "board.h"
#include <bitset>
#include <iostream>
#include <string>

void printBoard(const GameState &state) {
  std::string board = "";

  for (int square = 0; square < Board::TotalSquares; square++) {
    if (square % Board::RankSquares == 0) {
      board += "\n";
    }

    bool pieceOnSquare = false;
    for (Board::Piece piece = Board::Pawn; piece <= Board::King; piece++) {
      if (state.Bitboards[piece] & (1ul << square)) {
        auto pair = Board::PieceToChars.find(piece);
        if (pair != Board::PieceToChars.end()) {
          if ((1ul << square) & state.Bitboards[Board::White]) {
            board += std::toupper(pair->second);
          } else {
            board += pair->second;
          }
          pieceOnSquare = true;
        }
      }
    }

    if (!pieceOnSquare) {
      board += ".";
    }

    board += " ";
  }

  std::cerr << board << std::endl;
}

void printBitboards(const GameState &state) {
  for (Board::bitboard bitboard : state.Bitboards) {
    std::cerr << std::bitset<Board::TotalSquares>(bitboard) << std::endl;
  }
}
