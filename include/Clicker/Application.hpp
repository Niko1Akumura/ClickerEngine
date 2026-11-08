#pragma once

#include <Clicker/ResourceManager/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace Clicker {
class Application {
private:
  sf::RenderWindow m_window;
  sf::Clock m_clock;
  sf::CircleShape m_circle;
  float m_speed = 150.0f;

  Resource::Fonts m_fonts;
  Resource::Textures m_textures;
  Resource::Sounds m_sounds;

  void handlerEvent();
  void update(sf::Time dt);
  void draw();
  void initRes();

public:
  Application(sf::Vector2u screenSize, const std::string &title, float a);
  void run();
};
} // namespace Clicker
