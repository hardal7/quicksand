#include "../src/board/fen.h"
#include "../src/board/print.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>

TEST(MakeMoveTest, Castling) {
  Board::bitboard shortCastleOrigin = Board::FileH & Board::RankOne;
  Board::bitboard longCastleOrigin = Board::FileA & Board::RankOne;
  Board::bitboard shortCastleDestination = Board::FileF & Board::RankOne;
  Board::bitboard longCastleDestination = Board::FileC & Board::RankOne;

  using namespace Move;
  GameState state = loadFEN("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  printBoard(state);

  Move::Encoded shortCastle = encode(state, "e1g1");
  auto captured = make(state, shortCastle);
  printBoard(state);
  EXPECT_NE(state.Bitboards[Board::Rook] & shortCastleDestination, 0);
  EXPECT_EQ(state.Bitboards[Board::Rook] & shortCastleOrigin, 0);

  unmake(state, shortCastle, captured);
  printBoard(state);
  EXPECT_EQ(state.Bitboards[Board::Rook] & shortCastleDestination, 0);
  EXPECT_NE(state.Bitboards[Board::Rook] & shortCastleOrigin, 0);

  Move::Encoded longCastle = encode(state, "e1b1");
  captured = make(state, longCastle);
  printBoard(state);
  EXPECT_NE(state.Bitboards[Board::Rook] & longCastleDestination, 0);
  EXPECT_EQ(state.Bitboards[Board::Rook] & longCastleOrigin, 0);

  unmake(state, longCastle, captured);
  printBoard(state);
  EXPECT_EQ(state.Bitboards[Board::Rook] & longCastleDestination, 0);
  EXPECT_NE(state.Bitboards[Board::Rook] & longCastleOrigin, 0);
}

TEST(MakeMoveTest, EnPassant) {
  using namespace Move;
  GameState state = loadFEN("8/8/8/8/1p6/8/P7/8 w - - 0 1");

  Move::Encoded doublePawnMove = encode(state, "a2a4");
  auto captured = make(state, doublePawnMove);
  printBoard(state);

  Move::List movesList = generate(state);
  EXPECT_EQ(movesList.length, 2);

  Move::Encoded enPassantMove = NoMove;
  for (int i = 0; i < movesList.length; i++) {
    Readable m = decode(movesList.list[i]);
    if (m.DestinationSquare == Board::squareFromPosition("a3")) {
      enPassantMove = movesList.list[i];
    }
  }
  EXPECT_NE(enPassantMove, NoMove);

  make(state, enPassantMove);
  printBoard(state);
}

TEST(MakeMoveTest, EnPassantPrev) {
  using namespace Move;
  GameState state = loadFEN("8/8/8/8/1p6/8/P5N1/8 w - - 0 1");

  Move::Encoded doublePawnMove = encode(state, "a2a4");
  auto captured = make(state, doublePawnMove);
  printBoard(state);

  Move::Encoded knightMove = encode(state, "g2h4");
  auto capturedPiece = make(state, knightMove);
  unmake(state, knightMove, capturedPiece);

  Move::List movesList = generate(state);
  Move::Encoded enPassantMove = NoMove;
  for (int i = 0; i < movesList.length; i++) {
    Readable m = decode(movesList.list[i]);
    if (m.DestinationSquare == Board::squareFromPosition("a3")) {
      enPassantMove = movesList.list[i];
    }
  }
  EXPECT_NE(enPassantMove, NoMove);
}

TEST(MakeMoveTest, EnPassantMissed) {
  using namespace Move;
  GameState state = loadFEN("8/8/8/8/1p6/8/P5N1/8 w - - 0 1");

  Move::Encoded doublePawnMove = encode(state, "a2a4");
  auto captured = make(state, doublePawnMove);
  printBoard(state);

  Move::Encoded knightMove = encode(state, "g2h4");
  auto capturedPiece = make(state, knightMove);

  Move::List movesList = generate(state);
  Move::Encoded enPassantMove = NoMove;
  for (int i = 0; i < movesList.length; i++) {
    Readable m = decode(movesList.list[i]);
    if (m.DestinationSquare == Board::squareFromPosition("a3")) {
      enPassantMove = movesList.list[i];
    }
  }
  EXPECT_EQ(enPassantMove, NoMove);
}
