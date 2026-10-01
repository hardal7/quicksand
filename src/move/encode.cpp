#include "../../include/types.h"
#include "create.h"
#include <optional>
#include <string>

std::string decodeMove(int originSquare, int destinationSquare,
                       std::array<move, MAX_MOVES> &movesList,
                       std::optional<Board::Piece> promotionPiece = Board::None,
                       std::optional<bool> isCastle = false) {
  move m = 0;
  m |= originSquare << OriginSquareOffset;
  m |= destinationSquare << DestinationSquareOffset;
  if (promotionPiece != Board::None) {
    m |= promotionPiece.value() << PromotionPieceOffset;
    m |= PromotionFlag << FlagOffset;
  } else if (isCastle.value()) {
    m |= CastleFlag << FlagOffset;
  }

  return "";
}
