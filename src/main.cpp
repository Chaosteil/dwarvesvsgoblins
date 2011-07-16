#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "dvg_config.h"
#include "game/units/simple_unit_logic.h"
#include "game/wall_tile.h"
#include "graphics/components/simple_render_component.h"
#include "graphics/resource_manager.h"
#include "utils/game_object.h"
#include "utils/scene_manager.h"

using namespace dvg;

int main(int, const char **) {
  std::cout << "Dwarves vs. Goblins\nVersion: "
            << DVG_VERSION_STRING << std::endl;

  sf::VideoMode video_mode(800, 600, 32);
  sf::RenderWindow screen(video_mode, "Dwarves vs. Goblins", sf::Style::Close, 
			  sf::WindowSettings (32, 8, 0));
  sf::View view(sf::Vector2f(video_mode.Width / 2, video_mode.Height / 2),
                sf::Vector2f(video_mode.Width / 2, video_mode.Height / 2));
                
  screen.SetView(view);

  graphics::ResourceManager resource_manager;
  utils::SceneManager scene_manager;
  
  std::string tile_texture_name;
  utils::Vector2d tile_size(1.0f, 1.0f);
  utils::Vector2d tile_pos(0.0f, 0.0f);
  for (int y = 0; y < 50; y++) {
    for (int x = 0; x < 50; x++) {
      char tile_type = rand() % 2 + 1;
      
      if (tile_type == 1) {
        tile_texture_name = "tiles/dirt.png";
      } else if (tile_type == 2) {
        tile_texture_name = "tiles/grass.png";
      } else {
        tile_texture_name = "tiles/black.png";
      }

      game::WallTileLogic *logic
        = new game::WallTileLogic(tile_type);
      game::WallTileInput *input
        = new game::WallTileInput(*logic);
 
      graphics::SimpleRenderComponent *render = 
        new graphics::SimpleRenderComponent(
          resource_manager.GetTexture(tile_texture_name), screen);
      
      tile_pos.set_x(x);
      tile_pos.set_y(y);
      utils::GameObject *tile = 
        new utils::GameObject(scene_manager,
                              input, logic, render, NULL, 
                              utils::Rectangle(tile_pos, tile_size),
                              utils::Vector2d(0.0f, 0.0f),
                              0.0f);
      scene_manager.Attach(tile);
    }
  }
 
  graphics::SimpleRenderComponent *unit_render =
    new graphics::SimpleRenderComponent(
      resource_manager.GetTexture("tiles/grass.png"), screen);
  utils::GameObject *unit =
    new utils::GameObject(scene_manager,
                          NULL, new game::SimpleUnitLogic(), unit_render, NULL,
                          utils::Rectangle(utils::Vector2d(4.0, 4.0),
                                           tile_size),
                          utils::Vector2d(0, 0), 0);

  scene_manager.Attach(unit);

  sf::Event event;
  const sf::Input &input = screen.GetInput();
  bool running = true;
  while (running) {
      screen.Clear(sf::Color(0, 0, 0));
      
      while (screen.GetEvent(event)) {
        scene_manager.HandleInputEvent(event);

        if (event.Type == sf::Event::Closed) {
          running = false;
        } else if (event.Type == sf::Event::KeyPressed) {
          if (event.Key.Code == sf::Key::Escape) {
            running = false;
          } else if (event.Key.Code == sf::Key::A) {
            view.Move(-5, 0);
          } else if (event.Key.Code == sf::Key::D) {
            view.Move(5, 0);
          } else if (event.Key.Code == sf::Key::W) {
            view.Move(0, -5);
          } else if (event.Key.Code == sf::Key::S) {
            view.Move(0, 5);
          }
        }
      }

      scene_manager.HandleInput(input);
      scene_manager.Update();
      scene_manager.Render();
      screen.Display();
  }

  return 0;
}
