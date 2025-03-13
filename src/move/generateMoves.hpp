#ifndef GENERATE_MOVES_H
#define GENERATE_MOVES_H

#include <stdint.h>

#include "../utils/enums.hpp"

enum PawnMove {
  Single = 0, Double = 1, CaptureOne = 2, CaptureTwo = 3
};

enum Castle {
  Short = 0, Long = 1
};

const char kingMoves[8] = {-1, -7, -8, -9, 1, 7, 8, 9};
const char knightMoves[8] = {-6, -10, -15, -17, 6, 10, 15, 17};

const uint64_t kingMoveBorders[8] = {
  fileAMask, (fileHMask | rankEightMask),
  rankEightMask, (fileAMask | rankEightMask), 
  fileHMask, (fileAMask | rankOneMask),
  rankOneMask, (fileHMask | rankOneMask)
};

const uint64_t knightMoveBorders[8] = {
  (rankEightMask | fileGMask | fileHMask),
  (rankEightMask | fileAMask | fileBMask),
  (rankEightMask | rankSevenMask | fileHMask),
  (rankEightMask | rankSevenMask | fileAMask),

  (rankOneMask | fileAMask | fileBMask),
  (rankOneMask | fileGMask | fileHMask),
  (rankOneMask | rankTwoMask | fileAMask),
  (rankOneMask | rankTwoMask | fileHMask),
};

void initializeSlidingMoves(uint64_t bishopMoveMasks[262144], uint64_t rookMoveMasks[262144]);
unsigned int generatePsuedoLegalMoves(uint64_t bitboards[8], uint16_t movesList[128], uint16_t enPassantSquares, char castlingRights, char color);

#endif
