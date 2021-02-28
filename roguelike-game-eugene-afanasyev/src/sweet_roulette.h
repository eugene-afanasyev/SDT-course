#ifndef ROGUELIKE_GAME_SRC_SWEET_ROULETTE_H_
#define ROGUELIKE_GAME_SRC_SWEET_ROULETTE_H_

#include "character.h"
#include "dovahkiin.h"
#include "properties.h"
#include "enemy.h"

class SweetRoulette : public Character {
 public:
  SweetRoulette() {
    symbol_ = SWEET_ROULETTE_SYMBOL;
  }

  void Collide(Character *) override {}
  void Collide(Dovahkiin&) override {}
  void Collide(Draugr&) override {}
  void Collide(SweetRoulette&) override {}
  void Collide(Dragon&) override {}
  void Collide(Projectile&) override {}
  void Collide(Shell&) override {}
  void Collide(AidKit&) override {}
};

#endif //ROGUELIKE_GAME_SRC_SWEET_ROULETTE_H_
