#pragma once

#include "Clicker/Scene/IScene.hpp"
#include <Clicker/ResourceManager/Resources.hpp>
#include <Clicker/Scene/PongScene.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace Clicker {
class Application {
private:
  sf::RenderWindow m_window;
  sf::Clock m_clock;

  Resource::Fonts m_fonts;
  Resource::Textures m_textures;
  Resource::Sounds m_sounds;

  std::unique_ptr<Clicker::Scene::IScene> m_scene;

  void handlerEvent();
  void update(sf::Time dt);
  void draw();

  void initRes();
  void initScene();

public:
  Application(sf::Vector2u screenSize, const std::string &title);
  void run();
};
} // namespace Clicker
