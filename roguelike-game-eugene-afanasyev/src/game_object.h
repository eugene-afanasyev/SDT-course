#ifndef ROGUELIKE_GAME_SRC_GAME_OBJECT_H_
#define ROGUELIKE_GAME_SRC_GAME_OBJECT_H_

#include <memory>
#include "measurements.h"
#include "properties.h"

class GameObject {
 public:
  GameObject() : pos_(0, 0), symbol_(' ') {}
  explicit GameObject(char);

  virtual ~GameObject() = default;

  const Point2D &GetPos() const;
  char GetSymbol() const;

  void SetPos(const Point2D &);
  void SetSymbol(char);

 protected:
  Point2D pos_;
  char symbol_;
};

#endif //ROGUELIKE_GAME_SRC_GAME_OBJECT_H_
