#include "dragon.h"
#include "shell.h"

Dragon::Dragon() {
  symbol_ = DRAGON_SYMBOL;
  damage_ = 3;
  hp_ = 5;
  max_hp_ = 5;
}

void Dragon::Collide(Character *object) {
  object->Collide(*this);
}

void Dragon::Collide(Dovahkiin &dovahkiin) {
  dovahkiin.MakeDamage(damage_);
}

void Dragon::Collide(Shell &shell) {
  MakeDamage(shell.GetDamage());
  shell.SetHp(0);
}
