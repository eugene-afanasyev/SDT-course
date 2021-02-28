#ifndef ROGUELIKE_GAME_SRC_MEASUREMENTS_H_
#define ROGUELIKE_GAME_SRC_MEASUREMENTS_H_

#include <cstdlib>

struct Point2D {
  int x, y;

  Point2D(const Point2D &point) {
    x = point.x;
    y = point.y;
  }

  Point2D(int x, int y) {
    this->x = x;
    this->y = y;
  }

  bool operator==(const Point2D& other) const {
    return (x == other.x && y == other.y);
  }
};

struct Size2D {
  size_t width, height;

  Size2D() {
    width = 0;
    height = 0;
  }

  Size2D(size_t width, size_t height) {
    this->width = width;
    this->height = height;
  }
};

#endif //ROGUELIKE_GAME_SRC_MEASUREMENTS_H_
