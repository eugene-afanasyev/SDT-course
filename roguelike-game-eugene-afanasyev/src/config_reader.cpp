#include "config_reader.h"
#include <utility>

ConfigReader::ConfigReader() : file_path_(DEFAULT_CONFIG_PATH) {
  std::ifstream json_src_stream(file_path_);
  json_src_stream >> json_;
  json_src_stream.close();
}

Dovahkiin* ConfigReader::GetDovahkiin() {
  auto *dovahkiin = new Dovahkiin();
  json inner_json = json_["dovahkiin"];

  Point2D knight_pos(0,0);
  json pos_json = inner_json["pos"];
  knight_pos.x = pos_json["x"];
  knight_pos.y = pos_json["y"];

  dovahkiin->SetPos(knight_pos);
  dovahkiin->SetDamage(inner_json["damage"]);
  dovahkiin->SetMana(inner_json["mana"]);
  dovahkiin->SetMaxMana(inner_json["max-mana"]);
  dovahkiin->SetHp(inner_json["hp"]);
  dovahkiin->SetMaxHp(inner_json["max_hp"]);
  dovahkiin->SetSymbol(inner_json["symbol"].get<std::string>()[0]);
  return dovahkiin;
}

Draugr *ConfigReader::GetDraugr() {
  auto* draugr = new Draugr();
  json inner_json = json_["draugr"];

  draugr->SetDamage(inner_json["damage"]);
  draugr->SetHp(inner_json["hp"]);
  draugr->SetMaxHp(inner_json["max_hp"]);
  draugr->SetSymbol(inner_json["symbol"].get<std::string>()[0]);
  return draugr;
}

Dragon *ConfigReader::GetDragon() {
  auto* dragon = new Dragon();
  json draugr_json = json_["dragon"];

  dragon->SetDamage(draugr_json["damage"]);
  dragon->SetHp(draugr_json["hp"]);
  dragon->SetMaxHp(draugr_json["max_hp"]);
  dragon->SetSymbol(draugr_json["symbol"].get<std::string>()[0]);
  return dragon;
}

Necromancer* ConfigReader::GetNecromancer() {
  auto* necromancer = new Necromancer();
  json inner_json = json_["necromancer"];

  necromancer->SetDamage(inner_json["damage"]);
  necromancer->SetHp(inner_json["hp"]);
  necromancer->SetMaxHp(inner_json["max_hp"]);
  necromancer->SetSymbol(inner_json["symbol"].get<std::string>()[0]);
  return necromancer;
}

AidKit *ConfigReader::GetAidKit() {
  auto *aid_kit = new AidKit();
  json inner_json = json_["aid-kit"];

  aid_kit->SetDamage(inner_json["damage"]);
  aid_kit->SetHp(inner_json["hp"]);
  aid_kit->SetMaxHp(inner_json["max_hp"]);
  aid_kit->SetSymbol(inner_json["symbol"].get<std::string>()[0]);
  return aid_kit;
}

size_t ConfigReader::GetFPS() {
  std::ifstream json_src_stream(DEFAULT_CONFIG_PATH);
  json cfg_json;
  json_src_stream >> cfg_json;
  json_src_stream.close();
  return cfg_json["FPS"];
}
