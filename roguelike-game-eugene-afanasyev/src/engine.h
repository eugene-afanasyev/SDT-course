#ifndef ROGUELIKE_GAME_SRC_ENGINE_H_
#define ROGUELIKE_GAME_SRC_ENGINE_H_

#include <chrono>
#include <unistd.h>
#include <memory>
#include <set>
#include <map>
#include "game_object.h"
#include "game_map.h"
#include "dovahkiin.h"
#include "sweet_roulette.h"
#include "enemy.h"
#include "game_database.h"
#include "player_gui.h"
#include "shell.h"

enum GameStates {
  kOnPlay,
  kOnStop
};

class Engine {
 public:
  Engine();
  ~Engine();

  void Launch();

  void LoadObjects(const std::set<GameDatabase::ObjectPtr> &objects);

 private:
  GameDatabase game_database_;
  size_t level_;

  PlayerGUI gui_;

  size_t fps;
  size_t framerate_;
  int game_cycle_;

  int remaining_handling_time_{};

  mutable GameStates game_state_;

  void OverGame();
  void ReloadGame();
  void WinGame();
  void LoadNextLevel();
  void WinLevel();

  void Render() const;
  void HandleInput();
  void ActEnemies();
  void ActShells();
  void Collide();

  void RenderDovahkiin() const;
  void RenderMap() const;
  void RenderSweetRoulette() const;
  void RenderEnemies() const;
  void RenderKits() const;

  bool IsMovePermissible(const Character&, int, int);
};

#endif  //ROGUELIKE_GAME_SRC_ENGINE_H_
