#include "game_object.h"

const Point2D &GameObject::GetPos() const {
  return pos_;
}

char GameObject::GetSymbol() const {
  return symbol_;
}

void GameObject::SetPos(const Point2D &pos) {
  pos_ = pos;
}

void GameObject::SetSymbol(char symbol) {
  symbol_ = symbol;
}

GameObject::GameObject(char ch) : pos_(0, 0) {
  symbol_ = ch;
}
