#include "../src/board/fen.h"
#include "../src/move/generate.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>

TEST(MoveGenerationTest, Perft1) {
  GameState state = loadFEN();
  Move::List movesList = generateMoves(state);

  EXPECT_EQ(movesList.length, 20);
}
