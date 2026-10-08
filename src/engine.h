#include "game.h"
#include "search/transposition.h"

struct Engine {
  GameState state;
  int moveTimeSeconds;
  TTable ttable;
};
