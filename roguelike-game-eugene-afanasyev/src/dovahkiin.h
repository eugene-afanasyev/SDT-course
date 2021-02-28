#ifndef ROGUELIKE_GAME_SRC_DOVAHKIIN_H_
#define ROGUELIKE_GAME_SRC_DOVAHKIIN_H_

#include "properties.h"
#include "character.h"
#include "projectile.h"

class Dovahkiin : public Character  {
 public:
  Dovahkiin()  {
    symbol_ = DOVAHKIIN_SYMBOL;
    damage_ = 2;
    max_mana_ = 5;
  }

  ~Dovahkiin() override = default;

  void Heal(int);

  void Collide(Character *object) override;
  void Collide(Dovahkiin&) override {}
  void Collide(Draugr&) override;
  void Collide(SweetRoulette&) override {}
  void Collide(Dragon&) override;
  void Collide(Projectile&) override;
  void Collide(Shell&) override;
  void Collide(AidKit&) override;

  std::shared_ptr<Shell> ThrowShell();

  [[nodiscard]] int GetMana() const;
  [[nodiscard]] int GetMaxMana() const;

  void SetMana(int mana);
  void SetMaxMana(int max_mana);

  void SpendMana(int mana);
  void ReplenishMana(int mana);

 private:
  int mana_;
  int max_mana_;
};

#endif //ROGUELIKE_GAME_SRC_DOVAHKIIN_H_
