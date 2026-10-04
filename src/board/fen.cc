#include "../types.h"
#include <sstream>
#include <string>

void loadFEN(GameState &state, std::string fen) {
  std::istringstream in(fen);
  std::string pieces, colorToPlay, castlingRights, enPassantSquare,
      halfmoveClock, fullmoveNumber;
  in >> pieces >> colorToPlay >> castlingRights >> enPassantSquare >>
      halfmoveClock >> fullmoveNumber;

  for (auto c : pieces) {
    if (c == ' ') {
      break;
    }

    Board::Indexes color;
    int square = 0;

    char lowerC = std::tolower(c);
    if (c == lowerC) {
      color = Board::Black;
    } else {
      color = Board::White;
    }
    c = lowerC;

    auto pair = Piece::Chars.find(c);
    if (pair != Piece::Chars.end()) {
      state.Bitboards[color] |= (1ul << square);
      state.Bitboards[pair->second] |= (1ul << square);
    }

    else if (c != '/') {
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
}
