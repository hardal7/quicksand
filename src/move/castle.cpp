#include "../board/board.h"
#include "enums.h"

int shortCastleKingSquare(bool whiteToPlay) {
  return fileB + (whiteToPlay ? rankOne : rankEight);
}
int longCastleKingSquare(bool whiteToPlay) {
  return fileG + (whiteToPlay ? rankOne : rankEight);
}
int shortCastleRookSquare(bool whiteToPlay) {
  return fileH + (whiteToPlay ? rankOne : rankEight);
}
int longCastleRookSquare(bool whiteToPlay) {
  return fileA + (whiteToPlay ? rankOne : rankEight);
}
int unmovedKingSquare(bool whiteToPlay) {
  return fileE + (whiteToPlay ? rankOne : rankEight);
}

Board::Bitboard shortCastleRookPosition(bool whiteToPlay) {
  Board::Bitboard friendlyRank = whiteToPlay ? rankOneMask : rankEightMask;
  return fileHMask & friendlyRank;
}
Board::Bitboard shortCastleMask(bool whiteToPlay) {
  Board::Bitboard friendlyRank = whiteToPlay ? rankOneMask : rankEightMask;
  return fileFMask | fileGMask | friendlyRank;
}

Board::Bitboard longCastleRookPosition(bool whiteToPlay) {
  Board::Bitboard friendlyRank = whiteToPlay ? rankOneMask : rankEightMask;
  return fileAMask & friendlyRank;
}
Board::Bitboard longCastleMask(bool whiteToPlay) {
  Board::Bitboard friendlyRank = whiteToPlay ? rankOneMask : rankEightMask;
  return fileBMask | fileCMask | fileDMask | friendlyRank;
}
