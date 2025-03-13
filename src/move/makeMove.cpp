#include <stdint.h>

#include "makeMove.hpp"

void makeMove(uint64_t bitboards[8], uint16_t move, char color){
  char destinationSquare = (move & 0x3F);
  char originSquare = ((move & 0xFC0) >> 6);
  //Offset is set accordingly to: "enums.hpp" [1]
  char promotionPiece = (((move & 0x3000) + 3) >> 12);
  char specialMove = ((move & 0xC000) >> 14);
    
  auto typeOnSquare = [&](char squareIndex) -> char {
    //Offset is set accordingly to: "enums.hpp" [2]
    for (char pieceType = 2; pieceType < 8; pieceType++) {
      if (((1ul << squareIndex) & bitboards[pieceType]) != 0){ return pieceType; } 
      else {return 0;};}
  };

  //Promotion
  if (specialMove == 1){
    bitboards[color] ^= (1ul << originSquare);
    bitboards[color] |= (1ul <<destinationSquare);
    bitboards[typeOnSquare(originSquare)] ^= (1ul << originSquare);
    bitboards[promotionPiece] |= (1ul << destinationSquare);
    return;
  }

  //Change friendly bitboards
  for (unsigned int i = 0; i < 2; i++) {
    bitboards[color] ^= (1ul << (i == 0 ? originSquare : destinationSquare));
    bitboards[typeOnSquare(originSquare)] ^= (1ul << (i == 0 ? originSquare: destinationSquare));
  }

  //Change opponent bitboards (If it's a capture move)
  if ((1ul << destinationSquare) & bitboards[!color]){
    for (unsigned int i = 0; i < 2; i++) {
      bitboards[i == 0 ? !color : typeOnSquare(destinationSquare)] ^= (1ul << destinationSquare); }
  };
}
