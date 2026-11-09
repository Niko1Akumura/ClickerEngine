#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

namespace Clicker::Entity {
class Circle : public sf::CircleShape {
private:
  float m_speed = 150.0f;

public:
  explicit Circle(float r = 0.0f);
  void move(sf::Time dt);
  void reverse();
  void setSpeed(float speed);
  const float getSpeed();
};
} // namespace Clicker::Entity
