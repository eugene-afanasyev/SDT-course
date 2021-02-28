#include "draugr.h"
#include "dovahkiin.h"
#include "shell.h"

Draugr::Draugr() {
  symbol_ = DRAUGR_SYMBOL;
  hp_ = 2;
  max_hp_ = 2;
  damage_ = 1;
}

void Draugr::Collide(Character *object) {
  object->Collide(*this);
}

void Draugr::Collide(Dovahkiin& dovahkiin) {
  dovahkiin.MakeDamage(damage_);
}

void Draugr::Collide(Shell &shell) {
  MakeDamage(shell.GetDamage());
  shell.SetHp(0);
}
