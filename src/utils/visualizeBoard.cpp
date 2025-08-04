#include <cstdint>
#include <iostream>
#include <string>

#include "enums.hpp"

void visualizeBoard(uint64_t pieceBitboard[8]){
  for (int square=0; square<64; square++) {
    int piece = 0;
    for (int pieceType=Pawn; pieceType<=King; pieceType++) {
      if (pieceBitboard[pieceType] & (1ul << square)) {
        piece = pieceType;
      }
    } 
    for (int pieceColor=0; pieceColor<=Black; pieceColor+=Black) {
      if (pieceBitboard[pieceColor] & (1ul << square)) {
        piece += pieceColor;
      }
    } 

    std::string pieceString;
    switch (piece) {
      case (Black + Pawn):   pieceString = "p"; break;
      case (White + Pawn):   pieceString = "P"; break;
      case (Black + Knight): pieceString = "n"; break;
      case (White + Knight): pieceString = "N"; break;
      case (Black + Bishop): pieceString = "b"; break;
      case (White + Bishop): pieceString = "B"; break;
      case (Black + Rook):   pieceString = "r"; break;
      case (White + Rook):   pieceString = "R"; break;
      case (Black + Queen):  pieceString = "q"; break;
      case (White + Queen):  pieceString = "Q"; break;
      case (Black + King):   pieceString = "k"; break;
      case (White + King):   pieceString = "K"; break;
      
      default: pieceString = "."; break;
    }
    
    std::cout << pieceString << " ";
    if (square%8 == 7) {
      std::cout << "\n";
    }
  }
  std::cout << "\n";
}
