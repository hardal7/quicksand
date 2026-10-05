#include "../src/board/fen.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>

TEST(MoveGenTest, Perft1) {
  GameState state = loadFEN();
  Move::List movesList = Move::generate(state);

  EXPECT_EQ(movesList.length, 20);
}

TEST(MoveGenTest, Perft2) {
  GameState state = loadFEN();

  int positions = 0;
  Move::List movesList = Move::generate(state);

  for (int i = 0; i < movesList.length; i++) {
    Move::Encoded m = movesList.list[i];
    auto capturedPiece = Move::make(state, m);

    Move::List generatedList = Move::generate(state);
    positions += generatedList.length;

    Move::unmake(state, m, capturedPiece);
  }

  EXPECT_EQ(positions, 400);
}

TEST(MoveGenTest, PossibleCastles) {
  GameState state = loadFEN("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  Move::List movesList = Move::generate(state);

  bool shortCastle = false, longCastle = false;
  for (int i = 0; i < movesList.length; i++) {
    std::string m = Move::annotation(movesList.list[i]);
    if (m == "e1g1") {
      shortCastle = true;
    } else if (m == "e1b1") {
      longCastle = true;
    }
  }

  EXPECT_EQ(shortCastle, true);
  EXPECT_EQ(longCastle, true);
}

TEST(MoveGenTest, ObstructedCastling) {
  GameState state = loadFEN("8/8/8/8/8/8/8/R3K1NR w KQ - 0 1");
  Move::List movesList = Move::generate(state);

  bool shortCastle = false, longCastle = false;
  for (int i = 0; i < movesList.length; i++) {
    std::string m = Move::annotation(movesList.list[i]);
    if (m == "e1g1") {
      shortCastle = true;
    } else if (m == "e1b1") {
      longCastle = true;
    }
  }

  EXPECT_EQ(shortCastle, false);
  EXPECT_EQ(longCastle, true);
}
