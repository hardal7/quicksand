#include <stdint.h>

#include "popLSB.hpp"

uint64_t popLSB (uint64_t &bitboard){ 
  uint64_t lsb = bitboard & (~bitboard + 1);
  bitboard ^= lsb;
  return lsb;
}; 
