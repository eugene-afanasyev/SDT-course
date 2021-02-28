#ifndef ROGUELIKE_GAME_SRC_CHARACTER_H_
#define ROGUELIKE_GAME_SRC_CHARACTER_H_

#include <cstddef>
#include "measurements.h"
#include "game_object.h"

class Dovahkiin;
class Draugr;
class SweetRoulette;
class Dragon;
class Projectile;
class Shell;
class AidKit;

enum class MoveDirection {
  kTop = 0,
  kRight,
  kBottom,
  kLeft
};

class Character : virtual public GameObject {
 public:
  Character() : hp_(1), max_hp_(1), damage_(0) {}
  explicit Character(const Point2D &, size_t, size_t, size_t, char = '*');

  ~Character() override = default;

  [[nodiscard]] MoveDirection GetMoveDirection() const;

  void SetMoveDirection(const MoveDirection&);

  int GetHp() const;
  int GetMaxHp() const;
  int GetDamage() const;

  void SetHp(size_t);
  void SetMaxHp(size_t);
  void SetDamage(int);

  void Move(int x, int t);

  void MakeDamage(int);

  virtual void Collide(Character*) = 0;
  virtual void Collide(Dovahkiin&) = 0;
  virtual void Collide(Draugr&) = 0;
  virtual void Collide(SweetRoulette&) = 0;
  virtual void Collide(Dragon&) = 0;
  virtual void Collide(Projectile&) = 0;
  virtual void Collide(Shell&) = 0;
  virtual void Collide(AidKit&) = 0;

 protected:
  int hp_;
  int max_hp_;

  int damage_;

  MoveDirection move_direction_;
};

#endif //ROGUELIKE_GAME_SRC_CHARACTER_H_
