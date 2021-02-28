#include "player_gui.h"

PlayerGUI::PlayerGUI(
  const std::shared_ptr<Dovahkiin>& dovahkiin, const Size2D &map_size) :
  dov_ptr_(dovahkiin), map_size_(map_size) {}

void PlayerGUI::Print() const {
  PrintHP();
  PrintMana();
  PrintPos();
}

void PlayerGUI::PrintHP() const {
  mvaddstr(0, TERMINAL_SIZE_X - 15,"@@@@@@@@@@@@@@@");
  mvaddstr(1, TERMINAL_SIZE_X - 15,"|             |");
  mvaddstr(2, TERMINAL_SIZE_X - 15,"|             |");
  mvaddstr(3, TERMINAL_SIZE_X - 15,"|             |");
  mvaddstr(4, TERMINAL_SIZE_X - 15,"|_____________|");
  mvaddstr(1, TERMINAL_SIZE_X - 14,
           std::string("Health: " + std::to_string(dov_ptr_->GetHp())).c_str());
}

void PlayerGUI::PrintMana() const {
  mvaddstr(2, TERMINAL_SIZE_X - 14,
           std::string("Mana: " + std::to_string(dov_ptr_->GetMana())).c_str());
}

void PlayerGUI::PrintPos() const {
  mvaddstr(3, TERMINAL_SIZE_X -  14,
           std::string(
             "x: " + std::to_string(dov_ptr_->GetPos().x) + " "
             "y: " + std::to_string(dov_ptr_->GetPos().y)).c_str());
}

void PlayerGUI::SetDovPtr(const std::shared_ptr<Dovahkiin> &dov_ptr) {
  dov_ptr_ = dov_ptr;
}

void PlayerGUI::SetMapSize(const Size2D &map_size) {
  map_size_ = map_size;
}
