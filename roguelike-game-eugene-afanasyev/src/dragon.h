#ifndef ROGUELIKE_GAME_SRC_DRAGON_H_
#define ROGUELIKE_GAME_SRC_DRAGON_H_

#include "enemy.h"
#include "properties.h"
#include "sweet_roulette.h"

class Dragon : public Enemy {
 public:
  Dragon();

  void Collide(Character *) override;
  void Collide(Dovahkiin&) override;
  void Collide(Draugr&) override {}
  void Collide(SweetRoulette&) override {}
  void Collide(Dragon&) override {}
  void Collide(Projectile&) override {}
  void Collide(Shell&) override;
  void Collide(AidKit&) override {}
};

#endif //ROGUELIKE_GAME_SRC_DRAGON_H_
