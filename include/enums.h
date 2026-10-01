#pragma once

#include <cstdint>
#include <map>

const int MAX_MOVES = 256;

enum Masks : uint64_t {
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

enum FileAndRanks {
  fileA = 0,
  fileB = 1,
  fileC = 2,
  fileD = 3,
  fileE = 4,
  fileF = 5,
  fileG = 6,
  fileH = 7,

  rankOne = 56,
  rankTwo = 48,
  rankThree = 40,
  rankFour = 32,
  rankFive = 24,
  rankSix = 16,
  rankSeven = 8,
  rankEight = 0,
};

enum Directions {
  Up = -8,
  Down = 8,
  Left = -1,
  Right = 1,

  Forward = -1,
  Backward = 1,
};

const std::map<int, uint64_t> constraints = {
    {Up, rankEightMask},
    {Down, rankOneMask},
    {Left, fileAMask},
    {Right, fileHMask},

    {Up + Left, rankEightMask | fileAMask},
    {Up + Right, rankEightMask | fileHMask},
    {Down + Left, rankOneMask | fileAMask},
    {Down + Right, rankOneMask | fileHMask},

    {Up * 2 + Left, rankSevenMask | rankEightMask | fileAMask},
    {Up * 2 + Right, rankSevenMask | rankEightMask | fileHMask},
    {Down * 2 + Left, rankOneMask | rankTwoMask | fileAMask},
    {Down * 2 + Right, rankOneMask | rankTwoMask | fileHMask},

    {Up + Left * 2, fileAMask | fileBMask | rankEightMask},
    {Up + Right * 2, fileAMask | fileBMask | rankOneMask},
    {Down + Left * 2, fileGMask | fileHMask | rankEightMask},
    {Down + Right * 2, fileGMask | fileHMask | rankOneMask},
};
