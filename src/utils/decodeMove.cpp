#include <cstdint>
#include <string>

#include "enums.hpp"

uint16_t decodeMove(std::string moveString){
  uint16_t move = 0;
  int destinationSquare = 0, originSquare = 0, moveFlag = 0, promotionPiece = 0;
  int square = 0;
  
  for(int i=0; i<(int)moveString.size(); i++){
    switch (moveString[i]) {
      case 'a': square+=0; break;
      case 'b': square+=1; break;
      case 'c': square+=2; break;
      case 'd': square+=3; break;
      case 'e': square+=4; break;
      case 'f': square+=5; break;
      case 'g': square+=6; break;
      case 'h': square+=7; break;

      case '1': square+=56; break;
      case '2': square+=48; break;
      case '3': square+=40; break;
      case '4': square+=32; break;
      case '5': square+=24; break;
      case '6': square+=16; break;
      case '7': square+=8;  break;
      case '8': square+=0;  break;
    }

    if (i==4){
      moveFlag = Promotion;
      switch (moveString[i]) {
        case 'n': promotionPiece = Knight; break;
        case 'b': promotionPiece = Bishop; break;
        case 'r': promotionPiece = Rook;   break;
        case 'q': promotionPiece = Queen;  break;
      }
    }

  if(i<=1){ originSquare = square; }
  if(i==1) { square = 0; }
  else { destinationSquare = square; }
  }
  move = ((destinationSquare) | (originSquare << 6) | (moveFlag << 12) | ((promotionPiece + 2) << 14));
  return move;
}
