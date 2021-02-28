#ifndef ROGUELIKE_GAME_SRC_PLAYER_GUI_H_
#define ROGUELIKE_GAME_SRC_PLAYER_GUI_H_

#include <memory>
#include <ncurses.h>
#include "dovahkiin.h"

class PlayerGUI {
 public:
  PlayerGUI() = default;
  PlayerGUI(const std::shared_ptr<Dovahkiin>&, const Size2D&);

  void Print() const;

  void SetDovPtr(const std::shared_ptr<Dovahkiin> &dov_ptr);
  void SetMapSize(const Size2D &map_size);

 private:
  std::shared_ptr<Dovahkiin> dov_ptr_;
  Size2D map_size_;

  void PrintHP() const;
  void PrintPos() const;
  void PrintMana() const;
};

#endif //ROGUELIKE_GAME_SRC_PLAYER_GUI_H_
