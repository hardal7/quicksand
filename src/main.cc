#include "board/fen.h"
#include "types.h"

int main() {
  GameState state;
  loadFEN(state);
  return 0;
}
