#ifndef ROGUELIKE_GAME_SRC_CONFIG_READER_H_
#define ROGUELIKE_GAME_SRC_CONFIG_READER_H_

#include <fstream>
#include "character.h"
#include "dovahkiin.h"
#include "properties.h"
#include "draugr.h"
#include "dragon.h"
#include "projectile.h"
#include "necromancer.h"
#include "aid_kit.h"
#include "json/single_include/nlohmann/json.hpp"

class ConfigReader {
 public:
  using json = nlohmann::json;

  ConfigReader();

  Dovahkiin* GetDovahkiin();
  Draugr* GetDraugr();
  Dragon* GetDragon();
  Necromancer* GetNecromancer();
  AidKit* GetAidKit();

  static size_t GetFPS();

 private:
  std::string file_path_;
  json json_;
};

#endif //ROGUELIKE_GAME_SRC_CONFIG_READER_H_
