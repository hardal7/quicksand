#include <cstdint>
#include <string>

#include "enums.hpp"

int position = 0;

void loadFEN(uint64_t pieceBitboard[8], std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"){
  for(int i=0; fenString[i] != '\0'; i++){
    int pieceType = 0;
    switch (fenString[i]) {
      case '1': position += 0; break;
      case '2': position += 1; break;
      case '3': position += 2; break;
      case '4': position += 3; break;
      case '5': position += 4; break;
      case '6': position += 5; break;
      case '7': position += 6; break;
      case '8': position += 7; break;

      case 'p': pieceType = (Black + Pawn);   break;
      case 'P': pieceType = (White + Pawn);   break;
      case 'n': pieceType = (Black + Knight); break;
      case 'N': pieceType = (White + Knight); break;
      case 'b': pieceType = (Black + Bishop); break;
      case 'B': pieceType = (White + Bishop); break;
      case 'r': pieceType = (Black + Rook);   break;
      case 'R': pieceType = (White + Rook);   break;
      case 'q': pieceType = (Black + Queen);  break;
      case 'Q': pieceType = (White + Queen);  break;
      case 'k': pieceType = (Black + King);   break;
      case 'K': pieceType = (White + King);   break;

      case '/': position--; break;
    }
    position++;

    if (pieceType) {
      pieceBitboard[ (pieceType < Black) ? White : Black ] |= (1ul << (position-1));
      pieceBitboard[ pieceType - ((pieceType < Black) ? 0 : Black) ] |= (1ul << (position-1));
    }
  }
}
