#include <unistd.h>
#include "src/dovahkiin.h"
#include "src/engine.h"
#include "src/character.h"
#include "src/config_reader.h"
#include "src/level_factory.h"

using namespace std;

int main() {
  LevelFactory level_factory;
  level_factory.LoadLevel();

  Engine engine;
  engine.LoadObjects(level_factory.GetGameObjects());

  engine.Launch();
}