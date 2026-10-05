#include "../src/board/fen.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>
using namespace Move;

TEST(PerftTest, DepthOne) {
  GameState state = loadFEN();
  List movesList = generate(state);

  EXPECT_EQ(movesList.length, 20);
}

TEST(PerftTest, DepthTwo) {
  GameState state = loadFEN();

  int positions = 0;
  List movesList = generate(state);

  for (int i = 0; i < movesList.length; i++) {
    Encoded m = movesList.list[i];
    auto capturedPiece = make(state, m);

    List generatedList = generate(state);
    positions += generatedList.length;

    unmake(state, m, capturedPiece);
  }

  EXPECT_EQ(positions, 400);
}
