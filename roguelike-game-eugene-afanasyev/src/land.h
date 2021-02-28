#ifndef ROGUELIKE_GAME_SRC_LAND_H_
#define ROGUELIKE_GAME_SRC_LAND_H_

#include "properties.h"
#include "game_object.h"

class Land : public GameObject {
 public:
  Land() {
    symbol_ = LAND_SYMBOL;
  }
};

#endif //ROGUELIKE_GAME_SRC_LAND_H_
