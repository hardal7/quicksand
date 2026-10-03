#include "../src/board/board.h"
#include "../src/board/fen.h"
#include "../src/move/generate.h"
#include <gtest/gtest.h>

TEST(MoveGenerationTest, Perft1) {
  GameState state;
  loadFEN(state);

  std::array<ScoredMove, MAX_MOVES> movesList = {0};
  generateMoves(state, movesList);
  int numMoves = 0;
  for (auto m : movesList) {
    if (m.value == NO_MOVE) {
      break;
    }
    numMoves++;
  }

  EXPECT_EQ(numMoves, 20);
}
