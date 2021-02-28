#include <memory>
#include "game_map.h"
#include "properties.h"
#include "objects_factory.h"

GameMap::GameMap() : size_(TERMINAL_SIZE_X, TERMINAL_SIZE_Y),
objects_(TERMINAL_SIZE_Y, std::vector<object_ptr>(TERMINAL_SIZE_X)) {}

const Size2D &GameMap::GetSize() const {
  return size_;
}

const GameMap::map_type &GameMap::GetObjects() const {
  return objects_;
}

GameMap::object_ptr GameMap::GetCell(const Point2D &pt) const {
  return objects_[pt.y][pt.x];
}


GameMap::object_ptr GameMap::GetCell(int x, int y) const {
  return GetCell(Point2D(x, y));
}

void GameMap::SetCell(const Point2D& pt, const GameMap::object_ptr& obj) {
  objects_[pt.y][pt.x] = obj;
}

void GameMap::SetCell(int x, int y, const GameMap::object_ptr& obj) {
  SetCell(Point2D(x, y), obj);
}

std::vector<GameMap::object_ptr> GameMap::operator[](int i) {
  return objects_[i];
}
void GameMap::Clear() {
  for (auto vec : objects_) {
    vec.clear();
  }
}
