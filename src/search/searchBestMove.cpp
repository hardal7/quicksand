#include <cstdint>
#include <iostream>

#include "../utils/enums.hpp"
#include "../utils/annotateMove.hpp"
#include "../eval/evaluateBoard.hpp"
#include "../move/makeMove.hpp"
#include "../move/moveGeneration.hpp"
#include "../move/checkLegality.hpp"

int searchBestMove(uint64_t pieceBitboard[8], int color, int depth){
  uint16_t movesList[128] = {0};
  int movesNum = generateMoves(pieceBitboard, color, movesList);
  int bestEval = (color == White ? -1 : 1) * 100000;
  uint16_t bestMove = 0;
  for(int moveIndex=0; moveIndex<movesNum; moveIndex++){
    if (checkLegality(pieceBitboard, movesList[moveIndex], color)) {
      int capturedPiece = makeMove(pieceBitboard, movesList[moveIndex]);
    
      int eval = 0;
      if (depth) {
        eval = searchBestMove(pieceBitboard, color == White ? Black : White, (depth-1));     
      }
      else {
        eval = evaluateBoard(pieceBitboard); 
      }
      if (depth == maxDepth) {std::cout << "Found Move: " << annotateMove(movesList[moveIndex]) << " With Eval: " << eval << std::endl; }
      if (color == White ? (eval > bestEval) : (eval < bestEval)) {
        bestEval = eval; bestMove = movesList[moveIndex];
        if (depth==maxDepth){ bestMove = movesList[moveIndex]; }
      }

      makeMove(pieceBitboard, movesList[moveIndex], true, capturedPiece);
    }
  }
  if (depth!=maxDepth) { 
    return bestEval; 
  }
  else { return bestMove; }
}
