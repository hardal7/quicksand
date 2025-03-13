#ifndef MAGIC_BITBOARDS_H
#define MAGIC_BITBOARDS_H

#include <stdint.h>

void generateMoveMasks();
uint64_t getSlidingAttacks(unsigned int squareIndex, uint64_t allBitboard, char Piece);

#endif
