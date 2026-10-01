#include "encode.h"
#include "../../include/types.h"
#include "castle.h"
#include <string>
#include <utility>

std::pair<int, char> ranks[8] = {
    {1, '8'}, {2, '7'}, {3, '6'}, {4, '5'},
    {5, '4'}, {6, '3'}, {7, '2'}, {8, '1'},
};

std::pair<int, char> files[8] = {
    {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'},
    {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'},
};

std::string squareToPosition(int square) {
  std::string position = "";

  for (auto [val, c] : ranks) {
    if (val == square % RANK_SQUARES) {
      position += c;
      break;
    }
  }
  for (auto [val, c] : files) {
    if (val == RANK_SQUARES - square / RANK_SQUARES) {
      position += c;
      break;
    }
  }

  return position;
}

const std::map<int, char> pieces = {
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
    move += squareToPosition(i ? originSquare : destinationSquare);
  }

  if (flag == PromotionFlag) {
    move += pieces.at(promotionPiece);
  }

  return move;
}

int positionToSquare(std::string position) {
  int square = 0;

  for (auto [val, c] : files) {
    if (c == position[0]) {
      square += c;
      break;
    }
  }
  for (auto [val, c] : ranks) {
    if (c == position[2]) {
      square += val * (RANK_SQUARES - 1);
      break;
    }
  }

  return square;
}

move encodeMove(const GameState &state, std::string moveString) {
  move m = 0;

  int originSquare = 0, destinationSquare = 0;
  for (int i = 0; i < 2; i++) {
    i ? originSquare
      : destinationSquare =
            positionToSquare(moveString.substr(i ? 0 : 2, i ? 1 : 3));
  }

  Board::Piece promotionPiece = Board::None;
  if (moveString.length() > 4) {
    promotionPiece = pieces.at(promotionPiece);
  }

  bool isCastle =
      originSquare == unmovedKingSquare(state.whiteToPlay) &&
      (destinationSquare == shortcastleKingSquare(state.whiteToPlay) ||
       destinationSquare == longcastleKingSquare(state.whiteToPlay));

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
