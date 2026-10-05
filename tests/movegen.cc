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

TEST(MoveGenerationTest, Castling) {
  GameState state = loadFEN("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  Move::List movesList = generateMoves(state);

  bool shortCastle = false, longCastle = false;
  for (int i = 0; i < movesList.length; i++) {
    std::string m = Move::decode(movesList.list[i]);
    if (m == "e1g1") {
      shortCastle = true;
    } else if (m == "e1b1") {
      longCastle = true;
    }
  }

  EXPECT_EQ(shortCastle, true);
  EXPECT_EQ(longCastle, true);
}
