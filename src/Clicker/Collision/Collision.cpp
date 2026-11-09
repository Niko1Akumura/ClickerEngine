#include <Clicker/Collision/Collision.hpp>
#include <algorithm>

namespace Clicker::Collision {
bool Collision::hasCollisoin(const sf::Shape &m_ball,
                             const sf::Shape &m_paddle) {
  sf::Vector2f ball = m_ball.getPosition();
  sf::FloatRect paddleBounds = m_paddle.getGlobalBounds();

  float minX = paddleBounds.position.x;
  float maxX = paddleBounds.position.x + paddleBounds.size.x;
  float minY = paddleBounds.position.y;
  float maxY = paddleBounds.position.y + paddleBounds.size.y;

  float closestX = std::clamp(ball.x, minX, maxX);
  float closestY = std::clamp(ball.y, minY, maxY);

  float dx = ball.x - closestX;
  float dy = ball.y - closestY;

  float distSq = (dx * dx) + (dy * dy);
  float radius = m_ball.getGlobalBounds().size.x / 2.0f;

  return distSq < (radius * radius);
}
} // namespace Clicker::Collision
