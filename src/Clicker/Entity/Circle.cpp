#include <Clicker/Entity/Circle.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Clicker::Entity {

void Circle::move(sf::Time dt, float window) {
  float dCircle = this->getRadius() * 2.0f;
  float max_x = static_cast<float>(window - dCircle);

  sf::Vector2f pos = this->getPosition();

  if (pos.x >= max_x) {
    this->m_speed = -150.0f;
    this->setPosition(sf::Vector2f(max_x, pos.y));
  } else if (pos.x <= 0.0f) {
    this->m_speed = 150.0f;
    this->setPosition(sf::Vector2f(0.0f, pos.y));
  }
  sf::CircleShape::move(sf::Vector2f(this->m_speed * dt.asSeconds(), 0.0f));
}
} // namespace Clicker::Entity
