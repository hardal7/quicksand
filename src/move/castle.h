#pragma once

#include "../board/board.h"

int shortcastleKingSquare(bool whiteToPlay);
int longcastleKingSquare(bool whiteToPlay);
int unmovedKingSquare(bool whiteToPlay);

Board::Bitboard shortCastleRookPosition(bool whiteToPlay);
Board::Bitboard shortCastleMask(bool whiteToPlay);
Board::Bitboard longCastleRookPosition(bool whiteToPlay);
Board::Bitboard longCastleMask(bool whiteToPlay);
