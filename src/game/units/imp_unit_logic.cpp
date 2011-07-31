#include "game/units/imp_unit_logic.h"

#include "game/map.h"
#include "game/tiles/tile.h"
#include "utils/game_object.h"

namespace dvg {
namespace game {

ImpUnitLogic::ImpUnitLogic(Map &map) : map_(map), digging_(60) {}

ImpUnitLogic::~ImpUnitLogic() {}

void ImpUnitLogic::Update(utils::GameObject &game_object) {
  utils::Vector2d direction = target_ - game_object.position().pos();
  if (direction.length() > 1) {
    direction = direction.Normalize();
  }
  utils::Vector2d tile_pos = direction + game_object.position().pos();
  Tile *tile = map_.tile(tile_pos);

  if (tile == NULL) {
    return;
  }

  digging_++;

  if (tile->solid()) {
    if (digging_ >= 60) {
      tile->Dig();
      digging_ = 0;
    }
  } else {
    direction = direction * 0.01;
    utils::Vector2d new_pos = game_object.position().pos() + direction;
    new_pos.set_x(new_pos.x());
    new_pos.set_y(new_pos.y());
    game_object.set_pos(new_pos);
    game_object.set_velocity(direction);
  }
}

ImpUnitInput::ImpUnitInput(ImpUnitLogic &logic) : logic_(logic) {}

ImpUnitInput::~ImpUnitInput() {}

void ImpUnitInput::HandleInput(utils::GameObject &, const sf::Event &event) {
  if (event.Type != sf::Event::MouseMoved)
    return;
  
  utils::Vector2d target;
  target.set_x(event.MouseMove.X/64.0);
  target.set_y(event.MouseMove.Y/64.0);

  logic_.SetTargetTile(target);
}

}  // namespace game
}  // namespace dvg
