#pragma once

#include <SFML/Graphics/Shape.hpp>

namespace Clicker::Collision {
class Collision {
public:
  static bool hasCollisoin(const sf::Shape &ball, const sf::Shape &paddle);
};
} // namespace Clicker::Collision
