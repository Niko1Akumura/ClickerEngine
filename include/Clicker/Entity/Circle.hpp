#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

namespace Clicker::Entity {
class Circle {
private:
  sf::CircleShape m_shape;
  float m_speed = 150.0f;

public:
  Circle(float r);

  void setTexture(const sf::Texture *texture);
  void move(sf::Time dt, float Window);
  const sf::CircleShape &getShape() const;
};
} // namespace Clicker::Entity
