#ifndef ROGUELIKE_GAME_SRC_LEVEL_FACTORY_H_
#define ROGUELIKE_GAME_SRC_LEVEL_FACTORY_H_

#include <memory>
#include <string>
#include <memory>
#include "objects_factory.h"
#include "properties.h"
#include "game_map.h"
#include "config_reader.h"
#include "enemy.h"

class LevelFactory {
 public:
  using ObjectsSet = std::set<std::shared_ptr<GameObject>>;

  LevelFactory() : map_src_path_(DEFAULT_MAP_PATH) {}
  explicit LevelFactory(const std::string &path) : map_src_path_(path) {}

  void LoadLevel() {
    std::ifstream src_input(map_src_path_);

    for (int y = 0; y < TERMINAL_SIZE_Y; y++)
      for (int x = 0; x < TERMINAL_SIZE_X; x++) {
        char ch;
        src_input >> ch;

        auto object_ptr = std::shared_ptr<GameObject>(objects_factory_.Create(ch));
        object_ptr->SetPos(Point2D(x, y));
        game_objects_.insert(object_ptr);

        if (!dynamic_cast<Wall*>(object_ptr.get()) &&
          !dynamic_cast<Land*>(object_ptr.get())) {
          auto land = std::make_shared<Land>();
          land->SetPos(Point2D(x, y));
          game_objects_.insert(land);
        }

      }

    src_input.close();
  }

  void SetMapPath(const std::string& path) {
    map_src_path_ = path;
  }

  [[nodiscard]] const ObjectsSet& GetGameObjects() const {
    return game_objects_;
  }

 private:
  std::string map_src_path_;
  ObjectsFactory objects_factory_;

  ObjectsSet game_objects_;
};

#endif  // ROGUELIKE_GAME_SRC_LEVEL_FACTORY_H_
