#include "../src/board/fen.h"
#include "../src/board/print.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>
using namespace Move;

TEST(PromotionTest, GeneratePossible) {
  GameState state = loadFEN("8/P7/8/8/8/8/8/8 w - - 0 1");
  Move::List movesList = Move::generate(state);

  EXPECT_EQ(movesList.length, 4);
}

TEST(PromotionTest, UnmakeMove) {
  GameState state = loadFEN("8/P7/8/8/8/8/8/8 w - - 0 1");

  Encoded promotionMove = encode(state, "a7a8n");
  auto captured = make(state, promotionMove);
  printBoard(state);

  Board::bitboard allPieces = state.Bitboards[Board::White] | state.Bitboards[Board::Black];
  bool knightOnly = state.Bitboards[Board::Knight] == allPieces;
  EXPECT_EQ(knightOnly, true);

  unmake(state, promotionMove, captured);
  printBoard(state);

  allPieces = state.Bitboards[Board::White] | state.Bitboards[Board::Black];

  bool pawnOnly = state.Bitboards[Board::Pawn] == allPieces;
  EXPECT_EQ(pawnOnly, true);
  int piecesCount = __builtin_popcountll(allPieces);
  EXPECT_EQ(piecesCount, 1);
}

TEST(PromotionTest, MakeCaptureMove) {
  GameState state = loadFEN("1q6/P7/8/8/8/8/8/8 w - - 0 1");

  Encoded promotionMove = encode(state, "a7b8b");
  auto captured = make(state, promotionMove);
  printBoard(state);

  Board::bitboard allPieces = state.Bitboards[Board::White] | state.Bitboards[Board::Black];
  bool bishopOnly = state.Bitboards[Board::Bishop] == allPieces;
  EXPECT_EQ(bishopOnly, true);

  unmake(state, promotionMove, captured);
  printBoard(state);

  allPieces = state.Bitboards[Board::White] | state.Bitboards[Board::Black];
  Board::bitboard pawnAndQueen = state.Bitboards[Board::Pawn] | state.Bitboards[Board::Queen];

  bool pawnAndQueenOnly = pawnAndQueen == allPieces;
  EXPECT_EQ(pawnAndQueenOnly, true);
  int piecesCount = __builtin_popcountll(allPieces);
  EXPECT_EQ(piecesCount, 2);
}
