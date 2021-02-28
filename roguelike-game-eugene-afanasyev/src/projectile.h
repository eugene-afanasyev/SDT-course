#ifndef ROGUELIKE_GAME_SRC_PROJECTILE_H_
#define ROGUELIKE_GAME_SRC_PROJECTILE_H_

#include <memory>
#include "enemy.h"

class Projectile : public Enemy {
 public:
  Projectile();

  virtual std::shared_ptr<Shell> ThrowShell(MoveDirection direction, const Point2D&) = 0;

  void Collide(Character *) override;
  void Collide(Dovahkiin&) override;
  void Collide(Draugr&) override;
  void Collide(SweetRoulette&) override {}
  void Collide(Dragon&) override {}
  void Collide(Projectile&) override {}
  void Collide(Shell&) override;
  void Collide(AidKit&) override {}
};

#endif //ROGUELIKE_GAME_SRC_PROJECTILE_H_
