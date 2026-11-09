#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

namespace Clicker::Entity {
class Circle : public sf::CircleShape {
private:
  float m_speed = 150.0f;

public:
  using sf::CircleShape::CircleShape;
  void move(sf::Time dt, float Window);
  const sf::CircleShape &getShape() const;
};
} // namespace Clicker::Entity
