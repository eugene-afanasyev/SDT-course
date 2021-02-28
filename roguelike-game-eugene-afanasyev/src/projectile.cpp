#include "projectile.h"
#include "shell.h"
#include "draugr.h"
#include "dragon.h"

Projectile::Projectile() {
  hp_ = 1;
  max_hp_ = 1;
  damage_ = 1;
  symbol_ = PROJECTILE_SYMBOL;
}

void Projectile::Collide(Character *character) {
  character->Collide(*this);
  SetHp(0);
}

void Projectile::Collide(Dovahkiin &dovahkiin) {
  SetHp(0);
  dovahkiin.MakeDamage(1);
}

void Projectile::Collide(Draugr &draugr) {
  SetHp(0);
  draugr.MakeDamage(1);
}

void Projectile::Collide(Shell &shell) {
  SetHp(0);
  shell.SetHp(0);
}
