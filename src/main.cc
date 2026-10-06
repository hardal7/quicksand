#include "interface/uci.h"
#include "types.h"

int main() {
  GameState state;
  int moveTimeSeconds = 5;

  while (true) {
    handleUCI(state, moveTimeSeconds);
  }
}
