#include "dovahkiin.h"
#include "draugr.h"
#include "dragon.h"
#include "projectile.h"
#include "shell.h"
#include "aid_kit.h"

void Dovahkiin::Heal(int x) {
  hp_ = hp_ + x < max_hp_ ? hp_ + x : max_hp_;
}

void Dovahkiin::Collide(Character *object) {
  object->Collide(*this);
}

void Dovahkiin::Collide(Draugr& draugr) {
  hp_ -= draugr.GetDamage();
}

void Dovahkiin::Collide(Dragon &dragon) {
  hp_ -= dragon.GetDamage();
}

void Dovahkiin::Collide(Projectile &projectile) {
  MakeDamage(1);
  projectile.SetHp(0);
}

void Dovahkiin::Collide(Shell &shell) {
  hp_ -= shell.GetDamage();
  shell.SetHp(0);
}
std::shared_ptr<Shell> Dovahkiin::ThrowShell() {
  auto shell_ptr = std::make_shared<Shell>();
  shell_ptr->SetMoveDirection(move_direction_);
  switch (move_direction_) {
    case MoveDirection::kTop:
      shell_ptr->SetPos(Point2D(pos_.x, pos_.y - 1));
      break;
    case MoveDirection::kRight:
      shell_ptr->SetPos(Point2D(pos_.x + 1, pos_.y));
      break;
    case MoveDirection::kBottom:
      shell_ptr->SetPos(Point2D(pos_.x, pos_.y + 1));
      break;
    case MoveDirection::kLeft:
      shell_ptr->SetPos(Point2D(pos_.x - 1, pos_.y));
      break;
    default:
      break;
  }

  return shell_ptr;
}

void Dovahkiin::Collide(AidKit &aid_kit) {
  Heal(aid_kit.GetDamage());
  aid_kit.SetHp(0);
}

int Dovahkiin::GetMana() const {
  return mana_;
}

void Dovahkiin::SetMana(int mana) {
  mana_ = mana;
}

void Dovahkiin::SpendMana(int mana) {
  mana_ -= mana;
}

void Dovahkiin::ReplenishMana(int mana) {
  mana_ = mana_ + mana <= max_mana_ ? mana_ + mana : max_mana_;
}

int Dovahkiin::GetMaxMana() const {
  return max_mana_;
}

void Dovahkiin::SetMaxMana(int max_mana) {
  max_mana_ = max_mana;
}
