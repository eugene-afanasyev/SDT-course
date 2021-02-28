#ifndef ROGUELIKE_GAME_SRC_SHELL_H_
#define ROGUELIKE_GAME_SRC_SHELL_H_

#include "enemy.h"
#include "projectile.h"
#include "draugr.h"
#include "dragon.h"

class Shell : public Enemy {
 public:
  explicit Shell() {
    symbol_ = '*';
    damage_ = 2;
    hp_ = 1;
    max_hp_ = 1;
  }

  void Collide(Character *character) override {
    character->Collide(*this);
  }

  void Collide(Dovahkiin& dovahkiin) override {
    dovahkiin.MakeDamage(damage_);
    SetHp(0);
  }

  void Collide(Draugr& draugr) override {
    draugr.MakeDamage(damage_);
    SetHp(0);
  }

  void Collide(SweetRoulette&) override {}

  void Collide(Dragon& dragon) override {
    dragon.MakeDamage(damage_);
    SetHp(0);
  }

  void Collide(Projectile& projectile) override {
    projectile.MakeDamage(damage_);
    SetHp(0);
  }

  void Collide(Shell&) override {}

  void Collide(AidKit&) override {}
};

#endif //ROGUELIKE_GAME_SRC_SHELL_H_
