#include <stdint.h>
#include <string.h>

#include "loadFen.hpp"

enum Piece {
  Empty = 0, Pawn = 1, Knight = 2,
  Bishop = 3, Rook = 4, Queen = 5, King = 6, 

  White = 0, Black = 8,
};

void loadFEN(uint64_t bitboards[8], char fenString[]) {
  unsigned int piecePositions[64];
  unsigned int squareIndex = 0;
  
  //FEN string to piece array
  for (unsigned int i = 0; i < strlen(fenString); i++) {
    if (fenString[i] == '8' || fenString[i] == '7' || fenString[i] == '6' ||
        fenString[i] == '5' || fenString[i] == '4' || fenString[i] == '3' ||
        fenString[i] == '2' || fenString[i] == '1' || fenString[i] == '/') {
      for (int j = fenString[i] - '0'; j > 0; j--) {
        piecePositions[squareIndex] = Empty;
        squareIndex++;
      }
    }

    else {
      piecePositions[squareIndex] 
      = fenString[i] == 'P' ? (White | Pawn) 
      : fenString[i] == 'N' ? (White | Knight)
      : fenString[i] == 'B' ? (White | Bishop)
      : fenString[i] == 'R' ? (White | Rook)
      : fenString[i] == 'Q' ? (White | Queen)
      : fenString[i] == 'K' ? (White | King)
      : fenString[i] == 'p' ? (Black | Pawn)
      : fenString[i] == 'n' ? (Black | Knight)
      : fenString[i] == 'b' ? (Black | Bishop)
      : fenString[i] == 'r' ? (Black | Rook)
      : fenString[i] == 'q' ? (Black | Queen)
      : fenString[i] == 'k' ? (Black | King)
      : Empty;

      squareIndex++;
    }
  }

  //Piece array to bitboards
  for (unsigned int squareIndex = 0; squareIndex < 64; squareIndex++) {
    if (piecePositions[squareIndex] != Empty) {
      // Color bitboards
      bitboards[(piecePositions[squareIndex] < Black) ? 0 : 1] |= (1ul << squareIndex);

      // Piece bitboards
      bitboards[
      ( (piecePositions[squareIndex] == (White | Pawn)   || piecePositions[squareIndex] == (Black | Pawn))   ? 2
      : (piecePositions[squareIndex] == (White | Knight) || piecePositions[squareIndex] == (Black | Knight)) ? 3
      : (piecePositions[squareIndex] == (White | Bishop) || piecePositions[squareIndex] == (Black | Bishop)) ? 4
      : (piecePositions[squareIndex] == (White | Rook)   || piecePositions[squareIndex] == (Black | Rook))   ? 5
      : (piecePositions[squareIndex] == (White | Queen)  || piecePositions[squareIndex] == (Black | Queen))  ? 6
      : (piecePositions[squareIndex] == (White | King)   || piecePositions[squareIndex] == (Black | King))   ? 7
      : -1)] |= (1ul << squareIndex);
    }
  }
}
