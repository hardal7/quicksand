#include "interface/uci.h"

int main() {
  Engine engine;
  engine.moveTimeSeconds = 5;

  while (true) {
    handleUCI(engine);
  }
}
