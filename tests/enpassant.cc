#include "../src/board/fen.h"
#include "../src/board/print.h"
#include "../src/move/move.h"
#include "../src/types.h"
#include <gtest/gtest.h>
using namespace Move;

TEST(EnPassantTest, GeneratePossible) {
  GameState state = loadFEN("8/8/8/8/1p6/8/P7/8 w - - 0 1");

  Encoded doublePawnMove = encode(state, "a2a4");
  auto captured = make(state, doublePawnMove);
  printBoard(state);

  List movesList = generate(state);
  EXPECT_EQ(movesList.length, 2);

  Encoded enPassantMove = NoMove;
  for (int i = 0; i < movesList.length; i++) {
    Readable m = decode(movesList.list[i]);
    if (m.DestinationSquare == Board::squareFromPosition("a3")) {
      enPassantMove = movesList.list[i];
    }
  }
  EXPECT_NE(enPassantMove, NoMove);
}

TEST(EnPassantTest, GenerateMissed) {
  GameState state = loadFEN("8/8/8/8/1p6/8/P5N1/8 w - - 0 1");

  Encoded doublePawnMove = encode(state, "a2a4");
  auto captured = make(state, doublePawnMove);
  printBoard(state);

  Encoded knightMove = encode(state, "g2h4");
  auto capturedPiece = make(state, knightMove);

  List movesList = generate(state);
  Encoded enPassantMove = NoMove;
  for (int i = 0; i < movesList.length; i++) {
    Readable m = decode(movesList.list[i]);
    if (m.DestinationSquare == Board::squareFromPosition("a3")) {
      enPassantMove = movesList.list[i];
    }
  }
  EXPECT_EQ(enPassantMove, NoMove);
}

TEST(EnPassantTest, UnmakeMove) {
  GameState state = loadFEN("8/8/8/8/1p6/8/P5N1/8 w - - 0 1");

  Encoded doublePawnMove = encode(state, "a2a4");
  auto captured = make(state, doublePawnMove);
  printBoard(state);

  {
    Encoded knightMove = encode(state, "g2h4");
    auto capturedPiece = make(state, knightMove);
    unmake(state, knightMove, capturedPiece);
  }

  List movesList = generate(state);
  Encoded enPassantMove = NoMove;
  for (int i = 0; i < movesList.length; i++) {
    Readable m = decode(movesList.list[i]);
    if (m.DestinationSquare == Board::squareFromPosition("a3")) {
      enPassantMove = movesList.list[i];
    }
  }
  EXPECT_NE(enPassantMove, NoMove);
}
