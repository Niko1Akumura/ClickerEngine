#include <Clicker/Entity/Circle.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Clicker::Entity {
Circle::Circle(float r) : m_shape(r) {}

void Circle::move(sf::Time dt, float window) {
  float dCircle = this->m_shape.getRadius() * 2.0f;
  float max_x = static_cast<float>(window - dCircle);

  sf::Vector2f pos = this->m_shape.getPosition();

  if (pos.x >= max_x) {
    this->m_speed = -150.0f;
    this->m_shape.setPosition(sf::Vector2f(max_x, pos.y));
  } else if (pos.x <= 0.0f) {
    this->m_speed = 150.0f;
    this->m_shape.setPosition(sf::Vector2f(0.0f, pos.y));
  }
  this->m_shape.move(sf::Vector2f(this->m_speed * dt.asSeconds(), 0.0f));
}

void Circle::setTexture(const sf::Texture *texture) {
  this->m_shape.setTexture(texture);
}

const sf::CircleShape &Circle::getShape() const { return this->m_shape; }
} // namespace Clicker::Entity
