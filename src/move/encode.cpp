#include "encode.h"
#include "../../include/types.h"
#include "castle.h"
#include <string>
#include <utility>

const std::pair<int, char> ranks[8] = {
    {1, '8'}, {2, '7'}, {3, '6'}, {4, '5'},
    {5, '4'}, {6, '3'}, {7, '2'}, {8, '1'},
};

const std::pair<int, char> files[8] = {
    {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'},
    {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'},
};

std::string squareToPosition(int square) {
  std::string position = "";

  for (auto [val, c] : files) {
    if (val == square % RANK_SQUARES) {
      position += c;
      break;
    }
  }

  for (auto [val, c] : ranks) {
    if (val == (square / RANK_SQUARES) + 1) {
      position += c;
      break;
    }
  }

  return position;
}

const std::pair<int, char> pieces[4] = {
    {Board::Knight, 'n'},
    {Board::Bishop, 'b'},
    {Board::Rook, 'r'},
    {Board::Queen, 'q'},
};

std::string decodeMove(move m) {
  int originSquare = (m & OriginSquareOffsetMask) >> OriginSquareOffset;
  int destinationSquare =
      (m & DestinationSquareOffsetMask) >> DestinationSquareOffset;
  Board::Piece promotionPiece =
      (m & PromotionPieceOffsetMask) >> PromotionPieceOffset;
  int flag = (m & FlagOffsetMask) >> FlagOffset;

  std::string move = "";

  for (int i = 0; i < 2; i++) {
    move += squareToPosition(i ? destinationSquare : originSquare);
  }

  if (flag == PromotionFlag) {
    for (auto [val, c] : files) {
      if (val == promotionPiece) {
        move += c;
        break;
      }
    }
  }

  return move;
}

int positionToSquare(std::string position) {
  int square = 0;

  for (auto [val, c] : files) {
    if (c == position[0]) {
      square += val;
      break;
    }
  }
  for (auto [val, c] : ranks) {
    if (c == position[1]) {
      square += (val - 1) * RANK_SQUARES;
      break;
    }
  }

  return square;
}

move encodeMove(const GameState &state, std::string moveString) {
  int originSquare = 0, destinationSquare = 0;
  for (int i = 0; i < 2; i++) {
    (i ? originSquare : destinationSquare) =
        positionToSquare(moveString.substr(i ? 0 : 2, i ? 2 : 3));
  }

  Board::Piece promotionPiece = Board::None;
  if (moveString.length() > 4) {
    for (auto [val, c] : files) {
      if (c == moveString[4]) {
        promotionPiece = val;
        break;
      }
    }
  }

  bool isCastle =
      originSquare == unmovedKingSquare(state.whiteToPlay) &&
      (destinationSquare == shortcastleKingSquare(state.whiteToPlay) ||
       destinationSquare == longcastleKingSquare(state.whiteToPlay));

  move m = 0;
  m |= originSquare << OriginSquareOffset;
  m |= destinationSquare << DestinationSquareOffset;
  if (promotionPiece != Board::None) {
    m |= promotionPiece << PromotionPieceOffset;
    m |= PromotionFlag << FlagOffset;
  } else if (isCastle) {
    m |= CastleFlag << FlagOffset;
  }

  return m;
}
