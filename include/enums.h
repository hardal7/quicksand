#pragma once

#include <cstdint>

enum FileAndMasks : uint64_t {
  fileAMask = 0x0101010101010101,
  fileBMask = 0x0202020202020202,
  fileCMask = 0x0404040404040404,
  fileDMask = 0x0808080808080808,
  fileEMask = 0x1010101010101010,
  fileFMask = 0x2020202020202020,
  fileGMask = 0x4040404040404040,
  fileHMask = 0x8080808080808080,

  rankOneMask = 0xFF00000000000000,
  rankTwoMask = 0x00FF000000000000,
  rankThreeMask = 0x0000FF0000000000,
  rankFourMask = 0x000000FF00000000,
  rankFiveMask = 0x00000000FF000000,
  rankSixMask = 0x0000000000FF0000,
  rankSevenMask = 0x000000000000FF00,
  rankEightMask = 0x00000000000000FF,
};

enum Directions {
  Up = -8,
  Down = 8,
  Left = -1,
  Right = 1,

  Forward = -1,
  Backward = 1,
};
