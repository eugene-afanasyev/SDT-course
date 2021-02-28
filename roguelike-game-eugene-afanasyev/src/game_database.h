#ifndef ROGUELIKE_GAME_SRC_GAME_DATABASE_H_
#define ROGUELIKE_GAME_SRC_GAME_DATABASE_H_

#include <memory>
#include <vector>
#include <set>
#include <map>
#include "game_object.h"
#include "measurements.h"
#include "aid_kit.h"
#include "game_map.h"

class GameDatabase {
 public:
  using ObjectPtr = std::shared_ptr<GameObject>;

  GameDatabase() = default;
  ~GameDatabase() = default;

  [[nodiscard]] const std::set<std::shared_ptr<AidKit>> &GetKits() const;

  void Insert(const ObjectPtr&);
  void Clear();

  [[nodiscard]] const std::shared_ptr<Dovahkiin> &GetDovahkiin() const;
  [[nodiscard]] const std::shared_ptr<SweetRoulette> &GetSweetRoulette() const;
  [[nodiscard]] const std::set<std::shared_ptr<Enemy>> &GetEnemies() const;
  [[nodiscard]] const std::set<std::shared_ptr<Shell>> &GetShells() const;
  [[nodiscard]] const GameMap &GetGameMap() const;


  void SetDovahkiin(const std::shared_ptr<Dovahkiin> &dovahkiin);
  void SetSweetRoulette(const std::shared_ptr<SweetRoulette> &sweet_roulette);
  void SetEnemies(const std::set<std::shared_ptr<Enemy>> &enemies);
  void SetShells(const std::set<std::shared_ptr<Shell>> &shells);
  void SetKits(const std::set<std::shared_ptr<AidKit>> &kits);
  void SetGameMap(const GameMap &game_map);

  std::shared_ptr<Dovahkiin> &AccessDovahkiin();
  std::set<std::shared_ptr<Enemy>> &AccessEnemies();
  std::set<std::shared_ptr<Shell>> &AccessShells();

  void AddShell(const std::shared_ptr<Shell>&);

  void RemoveEnemy(const std::shared_ptr<Enemy>&);
  void RemoveShell(const std::shared_ptr<Shell>&);
  void RemoveKit(const std::shared_ptr<AidKit>&);

 private:
  std::shared_ptr<Dovahkiin> dovahkiin_;
  std::shared_ptr<SweetRoulette> sweet_roulette_;

  std::set<std::shared_ptr<Enemy>> enemies_;
  std::set<std::shared_ptr<Shell>> shells_;

  std::set<std::shared_ptr<AidKit>> kits_;

  GameMap game_map_;
};

#endif //ROGUELIKE_GAME_SRC_GAME_DATABASE_H_
