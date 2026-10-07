#include "../move/move.h"
#include <cstdint>

Move::Encoded convertPolyglotMove(uint16_t move) {
  Move::Readable m;

  int destinationFile = (move >> 0) & 0b111;
  int destinationRank = (move >> 3) & 0b111;
  int originFile = (move >> 6) & 0b111;
  int originRank = (move >> 9) & 0b111;
  int promotionPiece = (move >> 12) & 0b111;

  m.DestinationSquare = destinationFile + (Board::RankSquares - 1 - destinationRank) * Board::RankSquares;
  m.OriginSquare = originFile + (Board::RankSquares - 1 - originRank) * Board::RankSquares;
  m.PromotionPiece = Move::PromotionPiece(promotionPiece - 1);

  return Move::encode(m);
}
