#ifndef ROGUELIKE_GAME_SRC_DRAUGR_H_
#define ROGUELIKE_GAME_SRC_DRAUGR_H_

#include "enemy.h"
#include "properties.h"
#include "sweet_roulette.h"

class Draugr : public virtual Enemy {
 public:
  Draugr();

  void Collide(Character *) override;
  void Collide(Dovahkiin&) override;
  void Collide(Draugr&) override {}
  void Collide(SweetRoulette&) override {}
  void Collide(Dragon&) override {}
  void Collide(Projectile&) override {}
  void Collide(Shell&) override;
  void Collide(AidKit&) override{}
};

#endif //ROGUELIKE_GAME_SRC_DRAUGR_H_
