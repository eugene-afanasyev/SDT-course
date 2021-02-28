//
// Created by eugene on 08.01.2021.
//

#ifndef ROGUELIKE_GAME_SRC_WALL_H_
#define ROGUELIKE_GAME_SRC_WALL_H_

#include "game_object.h"
#include "properties.h"

class Wall : public virtual GameObject {
 public:
  Wall() {
    symbol_ = WALL_SYMBOL;
  }
};

#endif  //ROGUELIKE_GAME_SRC_WALL_H_
