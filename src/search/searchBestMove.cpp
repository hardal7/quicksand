#include <cstdint>
#include <iostream>

#include "../utils/enums.hpp"
#include "../utils/annotateMove.hpp"
#include "../eval/evaluateBoard.hpp"
#include "../move/makeMove.hpp"
#include "../move/moveGeneration.hpp"
#include "../move/checkLegality.hpp"

int boardPositions = 0;

int searchBestMove(uint64_t pieceBitboard[8], int color, int depth, int maxDepth, int alpha, int beta){
  uint16_t movesList[256] = {0};
  int movesNum = generateMoves(pieceBitboard, color, movesList);
  int bestEval = (color == White ? -infinity : infinity);
  uint16_t bestMove = 0;
  int eval = 0;

  for (int searchCaptures=true; searchCaptures>=false; searchCaptures--){
    for(int moveIndex=0; moveIndex<movesNum; moveIndex++){
      bool captureMove = (1ul << (movesList[moveIndex] & 0x3F)) & (pieceBitboard[White] | pieceBitboard[Black]);
      if (searchCaptures ? captureMove : !captureMove) {
        boardPositions++;
        if (checkLegality(pieceBitboard, movesList[moveIndex], color)) {
          int capturedPiece = makeMove(pieceBitboard, movesList[moveIndex]);

          if (depth != 0) { 
            eval = searchBestMove(pieceBitboard, color == White ? Black : White, (depth-1), maxDepth, alpha, beta); 
          }
          else { eval = evaluateBoard(pieceBitboard); }

          if (color == White ? (eval > bestEval) : (eval < bestEval)) { bestEval = eval; bestMove = movesList[moveIndex]; }

          if ((color == White) && (eval > alpha)) { alpha = eval; }
          else if ((color == Black) && (eval < beta)) { beta = eval; }
          if (alpha >= beta) { movesNum = 0; }
          if ((color == White) && (eval > beta)) { movesNum = 0; }
          if ((color == Black) && (eval < alpha)) { movesNum = 0; }

          makeMove(pieceBitboard, movesList[moveIndex], true, capturedPiece);
        }
      }
    }
  }
  if (depth != maxDepth) { return bestEval; }
  else { 
    std::cout << "Evaluated " << ((int)(boardPositions/10000) / 100.0) << "M positions" << std::endl;
    return bestMove; 
  }
}
