#ifndef ROGUELIKE_GAME_SRC_GAME_MAP_H_
#define ROGUELIKE_GAME_SRC_GAME_MAP_H_

#include <string>
#include <vector>
#include "game_object.h"
#include "measurements.h"
#include "properties.h"

class GameMap {
 public:
  using object_ptr = std::shared_ptr<GameObject>;
  using map_type = std::vector<std::vector<object_ptr>>;

  GameMap();

  virtual ~GameMap() = default;

  [[nodiscard]] const map_type &GetObjects() const;
  [[nodiscard]] const Size2D &GetSize() const;

  [[nodiscard]] object_ptr GetCell(const Point2D&) const;
  [[nodiscard]] object_ptr GetCell(int, int) const;

  void SetCell(const Point2D&, const object_ptr&);
  void SetCell(int, int, const object_ptr&);

  std::vector<object_ptr> operator[](int);

  void Clear();

 private:
  map_type objects_;
  Size2D size_;
};

#endif  //ROGUELIKE_GAME_SRC_GAME_MAP_H_
