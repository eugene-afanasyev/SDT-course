#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include "properties.h"
#include "engine.h"
#include "wall.h"
#include "land.h"
#include "draugr.h"
#include "dovahkiin.h"
#include "config_reader.h"
#include "level_factory.h"

Engine::Engine() : fps(ConfigReader::GetFPS()) {
  framerate_ = 1000 / fps;
  level_ = 1;
  game_cycle_ = 0;

  initscr();
  curs_set(0);
  noecho();

  // initializing color pairs
  start_color();
  init_color(COLOR_BLACK, 0, 0, 0);
  init_color(COLOR_GREEN, 0, 500, 0);
  init_color(COLOR_WHITE, 800, 800, 800);
  init_pair(MAP_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(PLAYER_COLOR_PAIR, COLOR_BLUE, COLOR_BLACK);
  init_pair(SWEET_ROULETTE_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
  init_pair(ENEMIES_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
  init_pair(AID_KIT_COLOR_PAIR, COLOR_GREEN, COLOR_WHITE);

  srand(time(0));

  remaining_handling_time_ = framerate_;

  gui_.SetMapSize(game_database_.GetGameMap().GetSize());
}

Engine::~Engine() {
  endwin();
}

void Engine::Launch() {
  timeout(framerate_);
  keypad(stdscr, TRUE);
  game_state_ = GameStates::kOnPlay;
  while (game_state_ == GameStates::kOnPlay) {
    Collide();
    ActEnemies();
    Collide();
    ActShells();
    Render();
    HandleInput();
    game_cycle_++;

    if (game_cycle_ % 6 == 0)
      game_database_.AccessDovahkiin()->ReplenishMana(1);
  }
}

void Engine::LoadObjects(const std::set<GameDatabase::ObjectPtr> &objects) {
  game_database_.Clear();
  for (auto& obj : objects)
    game_database_.Insert(obj);
  gui_.SetDovPtr(game_database_.GetDovahkiin());
}

void Engine::Render() const {
  // exactly in this order
  RenderMap();
  RenderDovahkiin();
  RenderSweetRoulette();
  RenderEnemies();
  RenderKits();
  gui_.Print();
}

void Engine::RenderMap() const {
  const auto game_map = game_database_.GetGameMap();
  attron(COLOR_PAIR(MAP_COLOR_PAIR));
  for (int y = 0; y < game_map.GetSize().height; y++)
    for (int x = 0; x < game_map.GetSize().width; x++)
    mvaddch(y, x, game_map.GetCell(x, y)->GetSymbol() | A_DIM);
  attroff(COLOR_PAIR(MAP_COLOR_PAIR));
}

void Engine::RenderDovahkiin() const {
  const auto& dovahkiin = game_database_.GetDovahkiin();
  attron(COLOR_PAIR(PLAYER_COLOR_PAIR));
  mvaddch(
    dovahkiin->GetPos().y, dovahkiin->GetPos().x,
    dovahkiin->GetSymbol() | A_BOLD);
  attroff(COLOR_PAIR(PLAYER_COLOR_PAIR));
}

void Engine::RenderSweetRoulette() const {
  const auto &sweet_roulette = game_database_.GetSweetRoulette();
  attron(COLOR_PAIR(SWEET_ROULETTE_COLOR_PAIR));
  mvaddch(
    sweet_roulette->GetPos().y, sweet_roulette->GetPos().x,
    sweet_roulette->GetSymbol() | A_BLINK | A_BOLD);
  attroff(COLOR_PAIR(SWEET_ROULETTE_COLOR_PAIR));
}


void Engine::RenderEnemies() const {
  const auto& enemies = game_database_.GetEnemies();
  for (auto& enemy : enemies) {
    attron(COLOR_PAIR(ENEMIES_COLOR_PAIR));
    mvaddch(enemy->GetPos().y, enemy->GetPos().x, enemy->GetSymbol() | A_BOLD);
    attroff(COLOR_PAIR(ENEMIES_COLOR_PAIR));
  }

  const auto& shells = game_database_.GetShells();
  for (auto& shell : shells) {
    attron(COLOR_PAIR(ENEMIES_COLOR_PAIR));
    mvaddch(shell->GetPos().y, shell->GetPos().x, shell->GetSymbol() | A_BOLD);
    attroff(COLOR_PAIR(ENEMIES_COLOR_PAIR));
  }
}

void Engine::RenderKits() const {
  const auto& kits = game_database_.GetKits();
  for (auto& kit : game_database_.GetKits()) {
    attron(COLOR_PAIR(AID_KIT_COLOR_PAIR));
    mvaddch(kit->GetPos().y, kit->GetPos().x, kit->GetSymbol() | A_BOLD);
    attroff(COLOR_PAIR(AID_KIT_COLOR_PAIR));
  }
}

void Engine::HandleInput() {
  auto dovahkiin = game_database_.AccessDovahkiin();
  auto start = std::chrono::high_resolution_clock::now();
  int ch = wgetch(stdscr);

  switch (ch) {
    case KEY_DOWN:
      if (IsMovePermissible(*dovahkiin, 0, 1))
        dovahkiin->Move(0, 1);
      dovahkiin->SetMoveDirection(MoveDirection::kBottom);
      break;
    case KEY_UP:
      if (IsMovePermissible(*dovahkiin, 0, -1))
        dovahkiin->Move(0, -1);
      dovahkiin->SetMoveDirection(MoveDirection::kTop);
      break;
    case KEY_LEFT:
      if (IsMovePermissible(*dovahkiin, -1,
                            0))
        dovahkiin->Move(-1, 0);
      dovahkiin->SetMoveDirection(MoveDirection::kLeft);
      break;
    case KEY_RIGHT:
      if (IsMovePermissible(*dovahkiin, 1, 0))
        dovahkiin->Move(1, 0);
      dovahkiin->SetMoveDirection(MoveDirection::kRight);
      break;
    case 32:
      if (dovahkiin->GetMana() > 0) {
        game_database_.AddShell(dovahkiin->ThrowShell());
        dovahkiin->SpendMana(1);
      }
      break;
    default:
      break;
  }

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  auto input_handling_time_ = duration.count();

  Render();
  Collide();

  remaining_handling_time_ -= input_handling_time_;
  if (remaining_handling_time_ > 0)
    HandleInput();
  else {
    remaining_handling_time_ = framerate_;
  }
}

bool Engine::IsMovePermissible(const Character& character, int x, int y) {
  auto possible_pos =
    Point2D(character.GetPos().x + x, character.GetPos().y + y);

  return (possible_pos.x >= 0 && possible_pos.x < TERMINAL_SIZE_X &&
          possible_pos.y >= 0 && possible_pos.y < TERMINAL_SIZE_Y &&
          game_database_.GetGameMap().GetCell(possible_pos)->GetSymbol() != WALL_SYMBOL);
}

void Engine::ActEnemies() {
  for (auto& enemy : game_database_.AccessEnemies()) {
    switch (enemy->GetMoveDirection()) {
      case MoveDirection::kTop:
        if (IsMovePermissible(*enemy, 0, -1))
          enemy->Move(0, -1);
        else
          enemy->SetMoveDirection(MoveDirection(rand() % 4));
        break;
      case MoveDirection::kRight:
        if (IsMovePermissible(*enemy, 1, 0))
          enemy->Move(1, 0);
        else
          enemy->SetMoveDirection(MoveDirection(rand() % 4));
        break;
      case MoveDirection::kBottom:
        if (IsMovePermissible(*enemy, 0, 1))
          enemy->Move(0, 1);
        else
          enemy->SetMoveDirection(MoveDirection(rand() % 4));
        break;
      case MoveDirection::kLeft:
        if (IsMovePermissible(*enemy, -1, 0))
          enemy->Move(-1, 0);
        else
          enemy->SetMoveDirection(MoveDirection(rand() % 4));
      default:break;
    }

    if (dynamic_cast<Projectile*>(enemy.get())) {
       auto shell = std::dynamic_pointer_cast<Projectile>(enemy)->
        ThrowShell(MoveDirection(rand() % 3), enemy->GetPos());
      if (rand() % 10 == 0) {
        game_database_.AddShell(shell);
      }
    }
  }
}

void Engine::ActShells() {
  for (auto& shell : game_database_.AccessShells()) {
    if (shell->GetDamage() != 2)
      exit(0);
    switch (shell->GetMoveDirection()) {
      case MoveDirection::kTop:
        if (IsMovePermissible(*shell, 0, -1))
          shell->Move(0, -1);
        else
          shell->SetHp(0);
        break;
      case MoveDirection::kRight:
        if (IsMovePermissible(*shell, 1, 0))
          shell->Move(1, 0);
        else
          shell->SetHp(0);
        break;
      case MoveDirection::kBottom:
        if (IsMovePermissible(*shell, 0, 1))
          shell->Move(0, 1);
        else
          shell->SetHp(0);
        break;
      case MoveDirection::kLeft:
        if (IsMovePermissible(*shell, -1, 0))
          shell->Move(-1, 0);
        else
          shell->SetHp(0);
      default:break;
    }
  }
}

void Engine::Collide() {
  auto& dovahkiin = game_database_.AccessDovahkiin();
  for (auto& enemy : game_database_.AccessEnemies()) {
    if (enemy->GetPos() == dovahkiin->GetPos()) {
      dovahkiin->Collide(enemy.get());
    }

    for (auto& shell : game_database_.AccessShells()) {
      if (shell->GetPos() == enemy->GetPos())
        shell->Collide(enemy.get());

      if (shell->GetPos() == dovahkiin->GetPos())
        shell->Collide(dovahkiin.get());

      if (game_database_.GetGameMap().GetCell(shell->GetPos())->GetSymbol() == WALL_SYMBOL)
        shell->SetHp(0);

      if (shell->GetHp() <= 0) {
        game_database_.RemoveShell(shell);
      }
    }

    if (enemy->GetHp() <= 0) {
      game_database_.RemoveEnemy(enemy);
    }
  }


  for (const auto& kit : game_database_.GetKits()) {
    if (dovahkiin->GetPos() == kit->GetPos())
      dovahkiin->Collide(kit.get());
  }

  for (auto& kit : game_database_.GetKits()) {
    if (kit->GetHp() == 0) {
      game_database_.RemoveKit(kit);
    }
  }

  if (dovahkiin->GetPos() == game_database_.GetSweetRoulette()->GetPos())
    WinLevel();

  if (dovahkiin->GetHp() <= 0)
    OverGame();
}

void Engine::OverGame() {
  timeout(-1);
  mvprintw(0, 0,
           "################################################################################"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                Game over...                                  #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                   Press enter to reload the game, or press                   #"
           "#                             any key to exit                                  #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "################################################################################");
  game_state_ = GameStates::kOnStop;
  auto ch = getch();
  if (ch == 10) {
    ReloadGame();
  } else {
    endwin();
    exit(0);
  }
}

void Engine::ReloadGame() {
  LevelFactory level_factory;
  level_factory.LoadLevel();
  LoadObjects(level_factory.GetGameObjects());
  game_state_ = GameStates::kOnPlay;
  level_ = 1;
  Launch();
}

void Engine::WinGame() {
  timeout(-1);
  mvprintw(0, 0,
           "################################################################################"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                               Congratulations!                               #"
           "#                               You won the game!                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                       Press escape key to exit the game                      #"
           "#                      or press any key to reload the game                     #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "################################################################################");
  game_state_ = GameStates::kOnStop;
  auto ch = getch();
  if (ch == 27) {
    endwin();
    exit(0);
  } else {
    ReloadGame();
  }
}

void Engine::LoadNextLevel() {
  timeout(-1);
  mvprintw(0, 0,
           "################################################################################"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                               Congratulations!                               #"
           "#                               You won this level!                            #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                       Press escape key to exit the game                      #"
           "#                      or press any key to load next level                     #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "#                                                                              #"
           "################################################################################");
  game_state_ = GameStates::kOnStop;
  auto ch = getch();
  if (ch == 27) {
    endwin();
    exit(0);
  } else {
    game_database_.Clear();

    game_state_ = GameStates::kOnStop;
    auto level_name = "map" + std::to_string(++level_) + ".dat";
    LevelFactory level_factory;
    level_factory.SetMapPath("./res/" + level_name);
    level_factory.LoadLevel();
    LoadObjects(level_factory.GetGameObjects());
    game_state_ = GameStates::kOnPlay;
    Launch();
  }
}

void Engine::WinLevel() {
  if (level_ != 3) {
    LoadNextLevel();
  } else {
    WinGame();
  }
}
