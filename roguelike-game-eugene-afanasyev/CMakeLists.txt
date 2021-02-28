cmake_minimum_required(VERSION 3.17)
project(roguelike_game)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -lncurses")


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main main.cpp src/measurements.h src/character.cpp src/character.h src/game_map.cpp src/game_map.h src/game_object.cpp src/game_object.h src/engine.cpp src/engine.h src/properties.h src/dovahkiin.h src/config_reader.cpp src/config_reader.h src/wall.h src/land.h src/objects_factory.h src/level_factory.h src/enemy.cpp src/enemy.h src/sweet_roulette.h src/draugr.cpp src/draugr.h src/game_database.cpp src/game_database.h src/dovahkiin.cpp src/player_gui.cpp src/player_gui.h src/dragon.cpp src/dragon.h src/projectile.cpp src/projectile.h src/necromancer.cpp src/necromancer.h src/shell.h src/aid_kit.h)
target_link_libraries(main ${CURSES_LIBRARIES})