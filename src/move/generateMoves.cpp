#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "generateMoves.hpp"
#include "magicBitboards.hpp"
#include "../utils/enums.hpp"
#include "../utils/popLSB.hpp"

/// bit 0-5:  Destination Square Index Of The Pawn That Can En Passant #1 (from 0 to 63)
/// bit 6-12: #2 (from 0 to 63) [Most amount of en passant moves in a single position is 2]
/// Castling Rights: Short White(1st bit), Long White (2nd bit), Short Black (3rd bit), Long Black (4th bit)

unsigned int generatePsuedoLegalMoves(uint64_t bitboards[8], uint16_t movesList[128], uint16_t enPassantSquares, char castlingRights, char color){

  //generateMoveMasks();
  unsigned int movesNum = 0;
  
  uint64_t allBitboard = (bitboards[White] | bitboards[Black]);
  uint64_t colorBitboard = bitboards[color];

  //================//
  //Helper Functions//
  //================// 
  
  //Check if there is a friendly piece of type (Piece) on squareIndex
  auto pieceOnSquare = [&](uint64_t squareBitboard, char Piece) -> bool { return ((bitboards[Piece] & bitboards[color]) & squareBitboard); };
  
  //Check if there is a friendly or enemy piece on move square (squareIndex + move) for given (color)
  auto typeOnSquare = [&](char squareIndex, char move, char color) -> bool { return ((1ul << (move + squareIndex)) & bitboards[color]); };

  //Check if the pawn has moved to generate double pawn move
  auto pawnHasMoved = [&](uint64_t squareBitboard) -> bool { return ((squareBitboard) & ~(color == White ? rankTwoMask : rankSevenMask)); };
  
  //Check if pawn can capture an enemy piece (O: CaptureOne, 1:CaptureTwo)
  auto pawnCanCapture = [&](uint64_t squareBitboard, char type) -> bool { 
    return !(squareBitboard & (color == White ? (type == 0 ? fileHMask : fileAMask) : (type == 0 ? fileAMask : fileHMask))); 
  };

  //Check if there is an obstructing piece on the way to generate double pawn move
  auto obstructingPawnPiece = [&](uint64_t squareBitboard) -> char {
    return (((squareBitboard << (color == White ? (-8) : (8) )) |
    (squareBitboard << (color == White ? (-16) : (16) ))) & (allBitboard));
  };
  
  //Check if there is an obstructing piece on the way to castle the king
  auto obstructingKingPiece = [&](char type) -> bool {
    return ((type == Short ? (fileFMask | fileGMask) : (fileBMask | fileCMask | fileDMask)) 
    & (color == White ? rankOneMask : rankEightMask) & allBitboard); 
  };
  
  //Generates en passant move for given piece
  auto generateEnPassant = [&](uint64_t squareBitboard) {
    //TODO
  };

  //Generate a move for given (squareIndex, move)
  auto generateMove = [&](char squareIndex, char move) {
    movesList[movesNum] |= (squareIndex + move); //Set the destination square
    movesList[movesNum] |= (squareIndex << 6); //Set the origin square
    movesNum++;
  };
  
  //Return pawn move for given move type (Single, Double, CaptureOne, CaptureTwo)
  auto pawnMove = [&](char type) -> char {
    switch (type) {
      case Single: return (color == White ? (-8) : (8));
      case Double: return (color == White ? (-16) : (16));
      case CaptureOne: return (color == White ? (-7) : (7));
      case CaptureTwo: return (color == White ? (-9) : (9));
      default: return 0;
    }
  };

  //Generate a pawn move with possible promotion for given (squareIndex, move)
  auto generatePawnMove = [&](uint64_t squareBitboard, char squareIndex, char move) {
    //Check if the pawn move is a promoting move
    if((squareBitboard & (color == White ? rankSevenMask : rankTwoMask))){
      //Iterate for all 4 possible promotion pieces
      for (unsigned long promotionPiece = 0; promotionPiece < 4; promotionPiece++) {
        movesList[movesNum] |= ((promotionPiece) << 12); //Set the promoted piece
        movesList[movesNum] |= (1ul << 14); //Set the promotion flag
        generateMove(squareIndex, move);
      }
    }
    else{ generateMove(squareIndex, move); }
  }; 

  auto castleKing = [&](char squareIndex, char type) {
    //3ul = 0011 and 12ul = 1100, we are setting the white and black castling rights as such
    movesList[movesNum] ^= ((color == White ? 3ul : 12ul) << 14); //Set the castling flag
    generateMove(squareIndex, (type == Short ? 2 : -2));
  };

  //===============//
  //Move Generation//
  //===============//
  
  //Iterate the squares of the board and generate moves for Piece on squareIndex
  for (unsigned int i = 0; i < __builtin_popcountll(bitboards[color]); i++) {
    uint64_t squareBitboard = popLSB(colorBitboard);
    char squareIndex = log2(squareBitboard);

    //Generate Pawn Moves
    if (pieceOnSquare(squareBitboard, Pawn)){
      //Single Move
      if(!(typeOnSquare(squareIndex, pawnMove(Single), color)) && !(typeOnSquare(squareIndex, pawnMove(Single), !color)))
      { generatePawnMove(squareBitboard, squareIndex, pawnMove(Single)); }
      //Double Move
      if(!pawnHasMoved(squareBitboard) && !obstructingPawnPiece(squareBitboard))
      { generatePawnMove(squareBitboard, squareIndex, pawnMove(Double)); }
      //Capture Moves
      for (unsigned int type = 0; type < 2; type++) {
        if(pawnCanCapture(squareBitboard, type) && (typeOnSquare(squareIndex, pawnMove((type == 0 ? CaptureOne : CaptureTwo)), !color)))
        { generatePawnMove(squareBitboard, squareIndex, pawnMove(type == 0 ? CaptureOne : CaptureTwo)); }
      }
      if(enPassantSquares){ generateEnPassant(squareBitboard); }
    }

    //Generate King & Knight Moves
    for (unsigned int i = 0; i < 2; i++) {
      //Alternate between knight and king to generate moves
      if (pieceOnSquare(squareBitboard, (i == 0 ? Knight : King))){
        //Iterate through all 8 possible moves
        for (unsigned int moveIndex = 0; moveIndex < 8; moveIndex++) {
          //Check if the piece is near a border
          if(!(typeOnSquare(squareIndex, (i == 0 ? knightMoves : kingMoves)[moveIndex], color)) &&
          (~(i == 0 ? knightMoveBorders : kingMoveBorders)[moveIndex] & squareBitboard) &&
          ((squareIndex + (i == 0 ? knightMoves : kingMoves)[moveIndex]) < 64))
          { generateMove(squareIndex, (i == 0 ? knightMoves : kingMoves)[moveIndex]); }
        }
        //Generate castle moves if possible
        //TODO: Check for checks using attack map
        for (unsigned int j = 0; j < 2; j++) {
          if (pieceOnSquare(squareBitboard, King) &&
          (castlingRights & (color == White ? (j == 0 ? 0x1 : 0x2) : (j == 0 ? 0x4 : 0x8))) && 
          !obstructingKingPiece(j == 0 ? Short : Long))
          { castleKing(squareIndex, (j == 0 ? Short : Long)); }
        }
      }
    }
    //Generate Bishop & Rook & Queen Moves
    //if (pieceOnSquare(squareBitboard, Bishop)){ generateBishopMoves(squareBitboard); }
    //if (pieceOnSquare(squareBitboard, Rook)){ generateRookMoves(squareBitboard); }
    //if (pieceOnSquare(squareBitboard, Queen)){ generateBishopMoves(squareBitboard); generateRookMoves(squareBitboard); } 
  }

  printf("Moves generated: %d\n\n", movesNum);
  return movesNum;
}
