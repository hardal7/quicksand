#include <cstdint>

const int maxDepth = 3;

enum Piece {
  Pawn   = 1,
  Knight = 2,
  Bishop = 3,
  Rook   = 4,
  Queen  = 5,
  King   = 6,

  White  = 0,
  Black  = 7,
};

enum MoveFlags {
  Promotion = 1,
  EnPassant = 2,
  Castling  = 3,
};

const uint64_t fileAMask = 0x0101010101010101;
const uint64_t fileBMask = 0x0202020202020202;
const uint64_t fileCMask = 0x0404040404040404;
const uint64_t fileDMask = 0x0808080808080808;
const uint64_t fileEMask = 0x1010101010101010;
const uint64_t fileFMask = 0x2020202020202020;
const uint64_t fileGMask = 0x4040404040404040;
const uint64_t fileHMask = 0x8080808080808080;

const uint64_t rankOneMask =   0xFF00000000000000;
const uint64_t rankTwoMask =   0x00FF000000000000;
const uint64_t rankThreeMask = 0x0000FF0000000000;
const uint64_t rankFourMask =  0x000000FF00000000;
const uint64_t rankFiveMask =  0x00000000FF000000;
const uint64_t rankSixMask =   0x0000000000FF0000;
const uint64_t rankSevenMask = 0x000000000000FF00;
const uint64_t rankEightMask = 0x00000000000000FF;

const uint64_t fileMasks[8] = {
  fileAMask, fileBMask, fileCMask, fileDMask,
  fileEMask, fileFMask, fileGMask, fileHMask
};

const uint64_t rankMasks[8] = {
  rankEightMask, rankSevenMask, rankSixMask, rankFiveMask,
  rankFourMask,  rankThreeMask, rankTwoMask, rankOneMask
};

const uint16_t kingSideCastleWhite  = (uint16_t)((0ul << 14) | (Castling << 12) | (60ul << 6) | (62));
const uint16_t queenSideCastleWhite = (uint16_t)((0ul << 14) | (Castling << 12) | (60ul << 6) | (58));
const uint16_t kingSideCastleBlack  = (uint16_t)((0ul << 14) | (Castling << 12) | (4ul << 6)  | (6));
const uint16_t queenSideCastleBlack = (uint16_t)((0ul << 14) | (Castling << 12) | (4ul << 6)  | (2));
