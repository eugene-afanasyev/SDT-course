#include "character.h"

void Character::SetMoveDirection(const MoveDirection &move_direction) {
  move_direction_ = move_direction;
}

MoveDirection Character::GetMoveDirection() const {
  return move_direction_;
}

int Character::GetHp() const {
  return hp_;
}

int Character::GetMaxHp() const {
  return max_hp_;
}

int Character::GetDamage() const {
  return damage_;
}

void Character::SetHp(size_t hp) {
  hp_ = hp;
}

void Character::SetMaxHp(size_t max_hp) {
  max_hp_ = max_hp;
}

void Character::SetDamage(int damage) {
  damage_ = damage;
}

Character::Character(const Point2D &pt, size_t hp, size_t max_hp, size_t damage, char ch) {
  pos_ = pt;

  hp_ = hp;
  max_hp_ = max_hp;

  damage_ = damage;
  symbol_ = ch;
}

void Character::Move(int x, int y) {
  pos_.x += x;
  pos_.y += y;
}

void Character::MakeDamage(int damage) {
  hp_ -= damage;
}
