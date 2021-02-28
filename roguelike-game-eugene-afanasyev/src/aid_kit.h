#ifndef ROGUELIKE_GAME_SRC_AID_KIT_H_
#define ROGUELIKE_GAME_SRC_AID_KIT_H_

#include "character.h"
#include "dovahkiin.h"

class AidKit : public Character {
 public:
  void Collide(Character *object) override {
    object->Collide(*this);
  }
  void Collide(Dovahkiin& dovahkiin) override {
    dovahkiin.Heal(damage_);
    SetHp(0);
  }
  void Collide(Draugr&) override {}
  void Collide(SweetRoulette&) override {}
  void Collide(Dragon&) override {}
  void Collide(Projectile&) override {}
  void Collide(Shell&) override {}
  void Collide(AidKit&) override {}
};

#endif //ROGUELIKE_GAME_SRC_AID_KIT_H_
