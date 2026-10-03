#pragma once

#include "../board/board.h"

int shortCastleKingSquare(bool whiteToPlay);
int shortCastleRookSquare(bool whiteToPlay);
int longCastleKingSquare(bool whiteToPlay);
int longCastleRookSquare(bool whiteToPlay);
int unmovedKingSquare(bool whiteToPlay);

Board::Bitboard shortCastleRookPosition(bool whiteToPlay);
Board::Bitboard shortCastleMask(bool whiteToPlay);
Board::Bitboard longCastleRookPosition(bool whiteToPlay);
Board::Bitboard longCastleMask(bool whiteToPlay);
