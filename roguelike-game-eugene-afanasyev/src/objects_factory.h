#ifndef ROGUELIKE_GAME_SRC_OBJECTS_FACTORY_H_
#define ROGUELIKE_GAME_SRC_OBJECTS_FACTORY_H_

#include <functional>
#include <map>
#include <exception>
#include "properties.h"
#include "game_object.h"
#include "land.h"
#include "wall.h"
#include "dovahkiin.h"
#include "sweet_roulette.h"
#include "draugr.h"
#include "necromancer.h"
#include "config_reader.h"

class ObjectsFactory {
 public:
  GameObject* Create(char ch) {
    if (obj_identifiers_.find(ch) == obj_identifiers_.end())
      throw std::invalid_argument("error: unresolved symbol found");

    auto obj = obj_identifiers_[ch]();
    return obj;
  }

 private:
  ConfigReader configurator_;

  std::map<char, std::function<GameObject*()>> obj_identifiers_ {
    {LAND_SYMBOL, []() -> Land* {
      auto tmp = new Land();
      return tmp;
    }},
    {WALL_SYMBOL, []() -> Wall* {
      auto tmp = new Wall();
      return tmp;
    }},
    {DOVAHKIIN_SYMBOL, [this]() -> Dovahkiin* {
      auto tmp = configurator_.GetDovahkiin();
      return tmp;
    }},
    {SWEET_ROULETTE_SYMBOL, []() -> SweetRoulette* {
      auto tmp = new SweetRoulette();
      return tmp;
    }},
    {DRAUGR_SYMBOL, [this]() -> Draugr* {
      auto tmp = configurator_.GetDraugr();
      return tmp;
    }},
    {DRAGON_SYMBOL, [this]() -> Dragon* {
      auto tmp = configurator_.GetDragon();
      return tmp;
    }},
    {NECROMANCER_SYMBOL, [this]() -> Necromancer* {
      auto tmp = configurator_.GetNecromancer();
      return tmp;
    }},
    {AID_KIT_SYMBOL, [this]() -> AidKit* {
      auto tmp = configurator_.GetAidKit();
      return tmp;
    }}
  };
};

#endif //ROGUELIKE_GAME_SRC_OBJECTS_FACTORY_H_
