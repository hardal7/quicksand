#include "../types.h"
#include "board.h"
#include <sstream>
#include <string>

GameState loadFEN(std::string fen) {
  GameState state;

  std::istringstream in(fen);
  std::string pieces, colorToPlay, castlingRights, enPassantSquare, halfmoveClock, fullmoveNumber;
  in >> pieces >> colorToPlay >> castlingRights >> enPassantSquare >> halfmoveClock >> fullmoveNumber;

  int square = 0;
  Board::Index color;

  for (char c : pieces) {
    if (c == '/') {
      continue;
    }

    char lowerC = std::tolower(c);
    if (c == lowerC) {
      color = Board::Black;
    } else {
      color = Board::White;
    }
    c = lowerC;

    auto pair = Board::PieceFromChars.find(c);
    if (pair != Board::PieceFromChars.end()) {
      state.Bitboards[color] |= (1ul << square);
      state.Bitboards[pair->second] |= (1ul << square);
      square++;
    }

    else {
      square += (c - '0');
    }
  }

  if (colorToPlay == "w") {
    state.WhiteToPlay = true;
  } else if (colorToPlay == "b") {
    state.WhiteToPlay = false;
  }

  else if (castlingRights == "QK") {
    state.WhiteLongCastle = true;
    state.WhiteShortCastle = true;
  } else if (castlingRights == "qk") {
    state.BlackLongCastle = true;
    state.BlackShortCastle = true;
  } else if (castlingRights == "Qk") {
    state.WhiteLongCastle = true;
    state.BlackShortCastle = true;
  } else if (castlingRights == "qK") {
    state.BlackLongCastle = true;
    state.WhiteShortCastle = true;
  }

  if (enPassantSquare != "-") {
    state.enPassantSquare = Board::squareFromPosition(enPassantSquare);
  }

  return state;
}
