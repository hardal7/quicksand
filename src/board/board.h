#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace Board {
const std::map<char, int> CharToSquares = {
    {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7},

    {'8', 0}, {'7', 1}, {'6', 2}, {'5', 3}, {'4', 4}, {'3', 5}, {'2', 6}, {'1', 7},
};

const std::map<int, char> CharFromSquares = {
    {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'},

    {0, '8'}, {1, '7'}, {2, '6'}, {3, '5'}, {4, '4'}, {5, '3'}, {6, '2'}, {7, '1'},
};

using bitboard = uint64_t;

const int Pieces = 6;
const int Colors = 2;

const int TotalSquares = 64;
const int FileSquares = 8;
const int RankSquares = 8;

using Piece = int;
enum Indexes : Piece { Pawn, Knight, Bishop, Rook, Queen, King, White, Black };

const std::map<char, Piece> PieceFromChars = {
    {'p', Pawn}, {'n', Knight}, {'b', Bishop}, {'r', Rook}, {'q', Queen}, {'k', King},
};
const std::map<Piece, char> PieceToChars = {
    {Pawn, 'p'}, {Knight, 'n'}, {Bishop, 'b'}, {Rook, 'r'}, {Queen, 'q'}, {King, 'k'},
};

int squareFromPosition(std::string position);
std::string squareToPosition(int square);

enum Directions { Up = -8, Down = 8, Left = -1, Right = 1 };
}; // namespace Board
