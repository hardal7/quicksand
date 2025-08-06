#include <cstdint>

#include "../utils/enums.hpp"
#include "../utils/annotateMove.hpp"

uint64_t returnPiece(uint64_t currentBitboard){
  return ((currentBitboard) & ~((currentBitboard)-1));
};

void createMove(
  uint16_t movesList[256], int &movesNum, uint64_t destinationSquareBitboard, uint64_t originSquareBitboard, int moveFlag = 0, int promotionPiece = 0){
  uint16_t move = 0;
  move = (((uint16_t)__builtin_ctzll(destinationSquareBitboard)) | (((uint16_t)__builtin_ctzll(originSquareBitboard)) << 6)
  | (moveFlag << 12) | (promotionPiece << 14));
  movesList[movesNum] = move;
  movesNum++;
}

void generatePawnMoves(uint64_t pieceBitboard[8], int color, uint64_t piecePosition, uint16_t movesList[256], int &movesNum){
  bool promotionMove = (piecePosition & (color == White ? rankSevenMask : rankTwoMask));
  if (!((color == White ? (piecePosition >> 8) : (piecePosition << 8))
  & (pieceBitboard[White] | pieceBitboard[Black]))){
    if (!promotionMove) { createMove(movesList, movesNum, (color == White ? (piecePosition >> 8) : (piecePosition << 8)), piecePosition); }
    else {
      for (int pieceType=Knight; pieceType<=Queen; pieceType++) {
        createMove(movesList, movesNum, (color == White ? (piecePosition >> 8) : (piecePosition << 8)), piecePosition, Promotion, pieceType);
      }
    }
  }
  if (!((color == White ? ((piecePosition >> 8) | (piecePosition >> 16)) : ((piecePosition << 8) | (piecePosition << 16)))
  & (pieceBitboard[White] | pieceBitboard[Black])) && (color == White ? (piecePosition >= (1ul << 48)) : (piecePosition < (1ul << 16)))){
    createMove(movesList, movesNum, (color == White ? (piecePosition >> 16) : (piecePosition << 16)), piecePosition);
  }
  for (int index = 7; index<=9; index+=2) {
    if (((color == White ? (piecePosition >> index) : (piecePosition << index)) & pieceBitboard[color == White ? Black : White]) &&
    !(((color == White) ? (index==9 ? fileAMask : fileHMask) : (index==9 ? fileHMask : fileAMask)) & piecePosition)){
      if (!promotionMove) { createMove(movesList, movesNum, (color == White ? (piecePosition >> index) : (piecePosition << index)), piecePosition); }
      else {
        for (int pieceType=Knight; pieceType<=Queen; pieceType++) {
          createMove(movesList, movesNum, (color == White ? (piecePosition >> index) : (piecePosition << index)), piecePosition, Promotion, pieceType);
        }
      }
    }
  }
}

void generateKnightMoves(uint64_t pieceBitboard[8], int color, uint64_t piecePosition, uint16_t movesList[256], int &movesNum){
  bool canMoveLeft = !(piecePosition & fileAMask);
  bool canMoveRight = !(piecePosition & fileHMask);
  bool canMoveDown = !(piecePosition & rankOneMask);
  bool canMoveUp = !(piecePosition & rankEightMask);

  bool canMoveLeftTwice = !(piecePosition & (fileAMask | fileBMask));
  bool canMoveRightTwice = !(piecePosition & (fileHMask | fileGMask));
  bool canMoveDownTwice = !(piecePosition & (rankOneMask | rankTwoMask));
  bool canMoveUpTwice = !(piecePosition & (rankEightMask | rankSevenMask));

  if (canMoveLeftTwice && canMoveUp && !(pieceBitboard[color] & (piecePosition >> 10)))
  { createMove(movesList, movesNum, (piecePosition >> 10), piecePosition); }
  if (canMoveLeftTwice && canMoveDown && !(pieceBitboard[color] & (piecePosition << 6)))
  { createMove(movesList, movesNum, (piecePosition << 6), piecePosition); }
  if (canMoveRightTwice && canMoveUp && !(pieceBitboard[color] & (piecePosition >> 6)))
  { createMove(movesList, movesNum, (piecePosition >> 6), piecePosition); }
  if (canMoveRightTwice && canMoveDown && !(pieceBitboard[color] & (piecePosition << 10)))
  { createMove(movesList, movesNum, (piecePosition << 10), piecePosition); }

  if (canMoveUpTwice && canMoveLeft && !(pieceBitboard[color] & (piecePosition >> 17)))
  { createMove(movesList, movesNum, (piecePosition >> 17), piecePosition); }
  if (canMoveUpTwice && canMoveRight && !(pieceBitboard[color] & (piecePosition >> 15)))
  { createMove(movesList, movesNum, (piecePosition >> 15), piecePosition); }
  if (canMoveDownTwice && canMoveLeft && !(pieceBitboard[color] & (piecePosition << 15)))
  { createMove(movesList, movesNum, (piecePosition << 15), piecePosition); }
  if (canMoveDownTwice && canMoveRight && !(pieceBitboard[color] & (piecePosition << 17)))
  { createMove(movesList, movesNum, (piecePosition << 17), piecePosition); }
}
  
void generateBishopMoves(uint64_t pieceBitboard[8], int color, uint64_t piecePosition, uint16_t movesList[256], int &movesNum){
  for (int direction=0; direction<2; direction++) {
    int piecePositionSquare = __builtin_ctzll(piecePosition);
    while (((piecePositionSquare / 8) != 0) && ((piecePositionSquare % 8) != (direction ? 0 : 7))) {
      uint64_t destinationPosition = (1ul << (piecePositionSquare) >> (direction ? 9 : 7));
      if (!(destinationPosition & ((pieceBitboard[White] | pieceBitboard[Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
      }
      else if (!(destinationPosition & ((pieceBitboard[color == White ? White : Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
        break;
      }
      else {
        break;
      }
      piecePositionSquare -= (direction ? 9 : 7);
    } 
    piecePositionSquare = __builtin_ctzll(piecePosition);
    while (((piecePositionSquare / 8) != 7) && ((piecePositionSquare % 8) != (direction ? 7 : 0))) {
      uint64_t destinationPosition = 1ul << (piecePositionSquare + (direction ? 9 : 7));
      if (!(destinationPosition & ((pieceBitboard[White] | pieceBitboard[Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
      }
      else if (!(destinationPosition & ((pieceBitboard[color == White ? White : Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
        break;
      }
      else {
        break;
      }
      piecePositionSquare += (direction ? 9 : 7);
    }
  }
}

void generateRookMoves(uint64_t pieceBitboard[8], int color, uint64_t piecePosition, uint16_t movesList[256], int &movesNum){
  for (int direction=0; direction<2; direction++) {
    int piecePositionSquare = __builtin_ctzll(piecePosition);
    while ((direction ? (piecePositionSquare % 8) : (piecePositionSquare / 8)) != 0) {
      uint64_t destinationPosition = (1ul << (piecePositionSquare) >> (direction ? 1 : 8));
      if (!(destinationPosition & ((pieceBitboard[White] | pieceBitboard[Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
      }
      else if (!(destinationPosition & ((pieceBitboard[color == White ? White : Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
        break;
      }
      else {
        break;
      }
      piecePositionSquare -= (direction ? 1 : 8);
    }
    piecePositionSquare = __builtin_ctzll(piecePosition);
    while ((direction ? (piecePositionSquare % 8) : (piecePositionSquare / 8)) != 7) {
      uint64_t destinationPosition = 1ul << (piecePositionSquare + (direction ? 1 : 8));
      if (!(destinationPosition & ((pieceBitboard[White] | pieceBitboard[Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
      }
      else if (!(destinationPosition & ((pieceBitboard[color == White ? White : Black]) ^ piecePosition))) {
        createMove(movesList, movesNum, destinationPosition, piecePosition);
        break;
      }
      else {
        break;
      }
      piecePositionSquare += (direction ? 1 : 8);
    }
  }
}

void generateKingMoves(uint64_t pieceBitboard[8], int color, uint64_t piecePosition, uint16_t movesList[256], int &movesNum){
  bool canMoveLeft = !(piecePosition & fileAMask);
  bool canMoveRight = !(piecePosition & fileHMask);
  bool canMoveDown = !(piecePosition & rankOneMask);
  bool canMoveUp = !(piecePosition & rankEightMask);

  if (canMoveLeft && !(pieceBitboard[color] & (piecePosition >> 1)))
  { createMove(movesList, movesNum, (piecePosition >> 1), piecePosition); }
  if (canMoveRight && !(pieceBitboard[color] & (piecePosition << 1)))
  { createMove(movesList, movesNum, (piecePosition << 1), piecePosition);}
  if (canMoveUp && !(pieceBitboard[color] & (piecePosition >> 8)))
  { createMove(movesList, movesNum, (piecePosition >> 8), piecePosition); }
  if (canMoveDown && !(pieceBitboard[color] & (piecePosition << 8)))
  { createMove(movesList, movesNum, (piecePosition << 8), piecePosition); }

  if (canMoveLeft && canMoveUp && !(pieceBitboard[color] & (piecePosition >> 9)))
  { createMove(movesList, movesNum, (piecePosition >> 9), piecePosition); }
  if (canMoveRight && canMoveUp && !(pieceBitboard[color] & (piecePosition >> 7)))
  { createMove(movesList, movesNum, (piecePosition >> 7), piecePosition); }
  if (canMoveLeft && canMoveDown && !(pieceBitboard[color] & (piecePosition << 7)))
  { createMove(movesList, movesNum, (piecePosition << 7), piecePosition); }
  if (canMoveRight && canMoveDown && !(pieceBitboard[color] & (piecePosition << 9)))
  { createMove(movesList, movesNum, (piecePosition << 9), piecePosition); }
    
  // TODO: This does not account for under threat castling which is an illegal move
  if (piecePosition & fileEMask & ((color==White) ? (rankOneMask) : (rankEightMask))) {
    if (!((pieceBitboard[White] | pieceBitboard[Black]) & (color == White ? rankOneMask : rankEightMask) & (fileBMask | fileCMask | fileDMask))){
      if(color == White ? ((pieceBitboard[White] & pieceBitboard[Rook]) & (1ul << 56)) : ((pieceBitboard[Black] & pieceBitboard[Rook]) & (1ul << 0))) {
        movesList[movesNum] = (color == White ? queenSideCastleWhite : queenSideCastleBlack);
        movesNum++;
      }
    }
    if (!((pieceBitboard[White] | pieceBitboard[Black]) & (color == White ? rankOneMask : rankEightMask) & (fileFMask | fileGMask))){
      if(color == White ? ((pieceBitboard[White] & pieceBitboard[Rook]) & (1ul << 63)) : ((pieceBitboard[Black] & pieceBitboard[Rook]) & (1ul << 7))) {
        movesList[movesNum] = (color == White ? kingSideCastleWhite : kingSideCastleBlack);
        movesNum++;
      }
    }
  }
}

int generateMoves(uint64_t pieceBitboard[8], int color, uint16_t movesList[256]){
  int movesNum = 0;
  for (int pieceType=Pawn; pieceType<=King; pieceType++) {
    uint64_t currentBitboard = (pieceBitboard[pieceType] & pieceBitboard[color]);
    uint64_t currentPiece = 0;

     do {
      currentBitboard ^= currentPiece;
      currentPiece = returnPiece(currentBitboard);
      if (currentPiece) {
        switch (pieceType) {
          case Pawn:   generatePawnMoves   (pieceBitboard, color, currentPiece, movesList, movesNum); break;
          case Knight: generateKnightMoves (pieceBitboard, color, currentPiece, movesList, movesNum); break;
          case Bishop: generateBishopMoves (pieceBitboard, color, currentPiece, movesList, movesNum); break;
          case Rook:   generateRookMoves   (pieceBitboard, color, currentPiece, movesList, movesNum); break;
          case Queen:  generateBishopMoves (pieceBitboard, color, currentPiece, movesList, movesNum);
                       generateRookMoves   (pieceBitboard, color, currentPiece, movesList, movesNum); break;
          case King:   generateKingMoves   (pieceBitboard, color, currentPiece, movesList, movesNum); break;
        }
      }
    } while(currentPiece);
  }
  return movesNum;
}
