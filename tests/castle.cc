#include "../src/board/fen.h"
#include "../src/board/print.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>
using namespace Move;

TEST(CastleTest, GeneratePossible) {
  GameState state = loadFEN("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  List movesList = generate(state);

  bool shortCastle = false, longCastle = false;
  for (int i = 0; i < movesList.length; i++) {
    std::string m = annotation(movesList.list[i]);
    if (m == "e1g1") {
      shortCastle = true;
    } else if (m == "e1b1") {
      longCastle = true;
    }
  }

  EXPECT_EQ(shortCastle, true);
  EXPECT_EQ(longCastle, true);
}

TEST(CastleTest, GenerateObstructed) {
  GameState state = loadFEN("8/8/8/8/8/8/8/R3K1NR w KQ - 0 1");
  List movesList = generate(state);

  bool shortCastle = false, longCastle = false;
  for (int i = 0; i < movesList.length; i++) {
    std::string m = annotation(movesList.list[i]);
    if (m == "e1g1") {
      shortCastle = true;
    } else if (m == "e1b1") {
      longCastle = true;
    }
  }

  EXPECT_EQ(shortCastle, false);
  EXPECT_EQ(longCastle, true);
}

TEST(CastleTest, MakeMove) {
  Board::bitboard shortCastleOrigin = Board::FileH & Board::RankOne;
  Board::bitboard longCastleOrigin = Board::FileA & Board::RankOne;
  Board::bitboard shortCastleDestination = Board::FileF & Board::RankOne;
  Board::bitboard longCastleDestination = Board::FileC & Board::RankOne;

  GameState state = loadFEN("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  printBoard(state);

  Encoded shortCastle = encode(state, "e1g1");
  auto captured = make(state, shortCastle);
  printBoard(state);
  EXPECT_NE(state.Bitboards[Board::Rook] & shortCastleDestination, 0);
  EXPECT_EQ(state.Bitboards[Board::Rook] & shortCastleOrigin, 0);

  unmake(state, shortCastle, captured);
  printBoard(state);
  EXPECT_EQ(state.Bitboards[Board::Rook] & shortCastleDestination, 0);
  EXPECT_NE(state.Bitboards[Board::Rook] & shortCastleOrigin, 0);

  Encoded longCastle = encode(state, "e1b1");
  captured = make(state, longCastle);
  printBoard(state);
  EXPECT_NE(state.Bitboards[Board::Rook] & longCastleDestination, 0);
  EXPECT_EQ(state.Bitboards[Board::Rook] & longCastleOrigin, 0);

  unmake(state, longCastle, captured);
  printBoard(state);
  EXPECT_EQ(state.Bitboards[Board::Rook] & longCastleDestination, 0);
  EXPECT_NE(state.Bitboards[Board::Rook] & longCastleOrigin, 0);
}
