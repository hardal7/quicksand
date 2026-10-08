#pragma once

#include "../game.h"

const std::map<Board::Piece, int> pieceValues = {
    {Board::Pawn, 100}, {Board::Knight, 350}, {Board::Bishop, 350},
    {Board::Rook, 525}, {Board::Queen, 1000}, {Board::King, 20000},
};

int evaluatePosition(const GameState &state);
