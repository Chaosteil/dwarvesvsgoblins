#include "utils/game_object.h"

#include "utils/game_object_manager.h"

namespace dvg {
namespace utils {

GameObject::GameObject(GameObjectManager &manager,
                       InputComponent *input_component,
                       LogicComponent *logic_component,
                       RenderComponent *render_component,
                       EventComponent *event_component,
                       Rectangle position,
                       Vector2d velocity,
                       double angle)
    : manager_(manager),
      input_component_(input_component), logic_component_(logic_component),
      render_component_(render_component), event_component_(event_component),
      position_(position), velocity_(velocity), angle_(angle), active_(true) {
  if (input_component_ != NULL) {
    input_component_->Init(*this);
  } else {
    input_component_ = new EmptyInputComponent();
  }

  if (logic_component_ != NULL) {
    logic_component_->Init(*this);
  } else {
    logic_component_ = new EmptyLogicComponent();
  }

  if (render_component_ != NULL) {
    render_component_->Init(*this);
  } else {
    render_component_ = new EmptyRenderComponent();
  }

  if (event_component_ != NULL) {
    event_component_->Init(*this);
  } else {
    event_component_ = new EmptyEventComponent();
  }
}

GameObject::~GameObject() {
  delete event_component_;
  delete render_component_;
  delete logic_component_;
  delete input_component_;
}

void GameObject::Destroy() {
  active_ = false;
  manager_.NotifyDestroyed(*this);
}

}  // namespace utils
}  // namespace dvg