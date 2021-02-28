#include "necromancer.h"

#include <memory>
#include "shell.h"

std::shared_ptr<Shell> Necromancer::ThrowShell(MoveDirection direction, const Point2D& pt) {
  auto shell_ptr = std::make_shared<Shell>();
  shell_ptr->SetMoveDirection(direction);
  switch (move_direction_) {
    case MoveDirection::kTop:
      shell_ptr->SetPos(Point2D(pos_.x, pos_.y - 1));
      break;
    case MoveDirection::kRight:
      shell_ptr->SetPos(Point2D(pos_.x + 1, pos_.y));
      break;
    case MoveDirection::kBottom:
      shell_ptr->SetPos(Point2D(pos_.x, pos_.y + 1));
      break;
    case MoveDirection::kLeft:
      shell_ptr->SetPos(Point2D(pos_.x - 1, pos_.y));
      break;
    default:
      break;
  }
  return shell_ptr;
}
