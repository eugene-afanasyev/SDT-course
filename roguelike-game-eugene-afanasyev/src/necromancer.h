#ifndef ROGUELIKE_GAME_SRC_NECROMANCER_H_
#define ROGUELIKE_GAME_SRC_NECROMANCER_H_

#include "projectile.h"

class Necromancer : public Projectile {
 public:
  Necromancer() {
    damage_ = 2;
    max_hp_ = 2;
    hp_ = 2;
    symbol_ = NECROMANCER_SYMBOL;
  }

  std::shared_ptr<Shell> ThrowShell(MoveDirection direction, const Point2D&) override;
};

#endif //ROGUELIKE_GAME_SRC_NECROMANCER_H_
