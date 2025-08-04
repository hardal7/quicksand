#include <cstdint>
#include <iostream>
#include <string>

#include "enums.hpp"

std::string annotateMove(uint16_t move){
  const int destinationSquare = (move & 0x3F);
  const int originSquare = ((move & 0xFC0) >> 6);
  const int moveFlag = ((move & 0x3000) >> 12);
  const int promotionPiece = (((move & 0xC000) >> 14) + 2);

  std::string moveString = "";
  for (int i=0; i<2; i++) {
    switch ((i==0 ? originSquare : destinationSquare) % 8) {
      case 0: moveString += "a"; break;
      case 1: moveString += "b"; break;
      case 2: moveString += "c"; break;
      case 3: moveString += "d"; break;
      case 4: moveString += "e"; break;
      case 5: moveString += "f"; break;
      case 6: moveString += "g"; break;
      case 7: moveString += "h"; break;
    }

    switch ((i==0 ? originSquare : destinationSquare) / 8) {
      case 0: moveString += "8"; break; 
      case 1: moveString += "7"; break; 
      case 2: moveString += "6"; break; 
      case 3: moveString += "5"; break; 
      case 4: moveString += "4"; break; 
      case 5: moveString += "3"; break; 
      case 6: moveString += "2"; break; 
      case 7: moveString += "1"; break; 
    }
  }

  if (moveFlag == Promotion) {
    switch (promotionPiece) {
      case Knight: moveString += "n"; break;
      case Bishop: moveString += "b"; break;
      case Rook:   moveString += "r"; break;
      case Queen:  moveString += "q"; break;
     } 
  }
  return moveString;
 }
