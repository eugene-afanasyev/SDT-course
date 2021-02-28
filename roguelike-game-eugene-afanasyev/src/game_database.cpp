#include <algorithm>
#include "game_database.h"
#include "dovahkiin.h"
#include "aid_kit.h"
#include "sweet_roulette.h"
#include "shell.h"


const std::set<std::shared_ptr<AidKit>> &GameDatabase::GetKits() const {
  return kits_;
}

void GameDatabase::Insert(const GameDatabase::ObjectPtr &object_ptr) {
  if (dynamic_cast<AidKit*>(object_ptr.get()))
    kits_.insert(std::dynamic_pointer_cast<AidKit>(object_ptr));
  else if (dynamic_cast<Dovahkiin*>(object_ptr.get()))
    dovahkiin_ = std::dynamic_pointer_cast<Dovahkiin>(object_ptr);
  else if (dynamic_cast<SweetRoulette*>(object_ptr.get()))
    sweet_roulette_ = std::dynamic_pointer_cast<SweetRoulette>(object_ptr);
  else if (dynamic_cast<Enemy*>(object_ptr.get()))
    enemies_.insert(std::dynamic_pointer_cast<Enemy>(object_ptr));
  else
    game_map_.SetCell(object_ptr->GetPos(), object_ptr);
}

void GameDatabase::Clear() {
  kits_.clear();
  enemies_.clear();
  shells_.clear();
  game_map_.Clear();
}

const std::shared_ptr<Dovahkiin> &GameDatabase::GetDovahkiin() const {
  return dovahkiin_;
}
const std::shared_ptr<SweetRoulette> &GameDatabase::GetSweetRoulette() const {
  return sweet_roulette_;
}
const std::set<std::shared_ptr<Enemy>> &GameDatabase::GetEnemies() const {
  return enemies_;
}
const std::set<std::shared_ptr<Shell>> &GameDatabase::GetShells() const {
  return shells_;
}
const GameMap &GameDatabase::GetGameMap() const {
  return game_map_;
}

void GameDatabase::SetDovahkiin(const std::shared_ptr<Dovahkiin> &dovahkiin) {
  dovahkiin_ = dovahkiin;
}
void GameDatabase::SetSweetRoulette(const std::shared_ptr<SweetRoulette> &sweet_roulette) {
  sweet_roulette_ = sweet_roulette;
}
void GameDatabase::SetEnemies(const std::set<std::shared_ptr<Enemy>> &enemies) {
  enemies_ = enemies;
}
void GameDatabase::SetShells(const std::set<std::shared_ptr<Shell>> &shells) {
  shells_ = shells;
}
void GameDatabase::SetKits(const std::set<std::shared_ptr<AidKit>> &kits) {
  kits_ = kits;
}
void GameDatabase::SetGameMap(const GameMap &game_map) {
  game_map_ = game_map;
}

std::shared_ptr<Dovahkiin> &GameDatabase::AccessDovahkiin() {
  return dovahkiin_;
}
std::set<std::shared_ptr<Enemy>> &GameDatabase::AccessEnemies() {
  return enemies_;
}
std::set<std::shared_ptr<Shell>> &GameDatabase::AccessShells() {
  return shells_;
}

void GameDatabase::RemoveKit(const std::shared_ptr<AidKit>& kit) {
  kits_.erase(kit);
}

void GameDatabase::RemoveEnemy(const std::shared_ptr<Enemy> &enemy) {
  enemies_.erase(enemy);
}

void GameDatabase::RemoveShell(const std::shared_ptr<Shell> &shell) {
  shells_.erase(shell);
}
void GameDatabase::AddShell(const std::shared_ptr<Shell> &shell) {
  shells_.insert(shell);
}
