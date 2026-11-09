#include <Clicker/Entity/Circle.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace Clicker::Entity {

Circle::Circle(float r) : sf::CircleShape(r) {
  sf::CircleShape::setOrigin(sf::Vector2f(
      {sf::CircleShape::getRadius(), sf::CircleShape::getRadius()}));
}

void Circle::move(sf::Time dt) {
  sf::CircleShape::move(sf::Vector2f(this->m_speed * dt.asSeconds(), 0.0f));
}

void Circle::reverse() { this->m_speed = -m_speed; }
void Circle::setSpeed(float speed) { this->m_speed = speed; }
const float Circle::getSpeed() { return this->m_speed; }
} // namespace Clicker::Entity
