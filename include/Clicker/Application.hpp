#pragma once

#include <Clicker/Entity/Circle.hpp>
#include <Clicker/ResourceManager/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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

  sf::RectangleShape m_paddle;
  sf::RectangleShape m_paddleL;

  Entity::Circle m_circle;

  void handlerEvent();
  void update(sf::Time dt);
  void draw();

  void initRes();
  void initEntity();

public:
  int count = 0;
  Application(sf::Vector2u screenSize, const std::string &title);
  void run();
};
} // namespace Clicker
