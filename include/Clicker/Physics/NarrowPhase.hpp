#pragma once

#include <Clicker/Physics/Collider.hpp>
#include <Clicker/Physics/Common.hpp>
#include <cmath>

namespace Clicker::Physics {
class NarrowPhase {
private:
  static float clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
  }

public:
  static Manifold testCircleCircle(const CircleShape &a, const CircleShape &b) {
    Manifold m;
    sf::Vector2f diff = b.center - a.center;
    float distSq = diff.x * diff.x + diff.y * diff.y;
    float radiusSum = a.radius + b.radius;

    if (distSq >= radiusSum * radiusSum) {
      return m;
    }

    float distance = std::sqrt(distSq);
    m.hasCollision = true;
    if (distance != 0.0f) {
      m.penetration = radiusSum - distance;
      m.normal = diff / distance;
    } else {
      m.penetration = a.radius;
      m.normal = sf::Vector2f(1.0f, 0.0f);
    }
    return m;
  }

  static Manifold testBoxBox(const BoxShape &a, const BoxShape &b) {
    Manifold m;

    sf::Vector2f centerA(a.rect.position.x + a.rect.size.x / 2.0f,
                         a.rect.position.y + a.rect.size.y / 2.0f);
    sf::Vector2f centerB(b.rect.position.x + b.rect.size.x / 2.0f,
                         b.rect.position.y + b.rect.size.y / 2.0f);

    sf::Vector2f delta = centerB - centerA;

    float overlapX =
        (a.rect.size.x / 2.0f + b.rect.size.x / 2.0f) - std::abs(delta.x);
    if (overlapX <= 0.0f)
      return m;

    float overlapY =
        (a.rect.size.y / 2.0f + b.rect.size.y / 2.0f) - std::abs(delta.y);
    if (overlapY <= 0.0f)
      return m;

    m.hasCollision = true;

    if (overlapX < overlapY) {
      m.penetration = overlapX;
      m.normal = (delta.x > 0.0f) ? sf::Vector2f(1.0f, 0.0f)
                                  : sf::Vector2f(-1.0f, 0.0f);
    } else {
      m.penetration = overlapY;
      m.normal = (delta.y > 0.0f) ? sf::Vector2f(0.0f, 1.0f)
                                  : sf::Vector2f(0.0f, -1.0f);
    }

    return m;
  }

  static Manifold testCircleBox(const CircleShape &circle,
                                const BoxShape &box) {
    Manifold m;

    float closestX = clamp(circle.center.x, box.rect.position.x,
                           box.rect.position.x + box.rect.size.x);
    float closestY = clamp(circle.center.y, box.rect.position.y,
                           box.rect.position.y + box.rect.size.y);

    sf::Vector2f diff = sf::Vector2f(closestX, closestY) - circle.center;
    float distSq = diff.x * diff.x + diff.y * diff.y;

    if (distSq > circle.radius * circle.radius) {
      return m;
    }

    m.hasCollision = true;
    float distance = std::sqrt(distSq);

    if (distance != 0.0f) {
      m.penetration = circle.radius - distance;
      m.normal = diff / distance;
    } else {
      sf::Vector2f centerBox(box.rect.position.x + box.rect.size.x / 2.0f,
                             box.rect.position.y + box.rect.size.y / 2.0f);
      sf::Vector2f delta = circle.center - centerBox;

      float overlapX = (box.rect.size.x / 2.0f) - std::abs(delta.x);
      float overlapY = (box.rect.size.y / 2.0f) - std::abs(delta.y);

      if (overlapX < overlapY) {
        m.penetration = circle.radius + overlapX;
        m.normal = (delta.x >= 0.0f) ? sf::Vector2f(-1.0f, 0.0f)
                                     : sf::Vector2f(1.0f, 0.0f);
      } else {
        m.penetration = circle.radius + overlapY;
        m.normal = (delta.y >= 0.0f) ? sf::Vector2f(0.0f, -1.0f)
                                     : sf::Vector2f(0.0f, 1.0f);
      }
    }

    return m;
  }

  static Manifold testCollision(const Collider &a, const Collider &b) {
    return std::visit(
        [&](const auto &shapeA, const auto &shapeB) -> Manifold {
          using TypeA = std::decay_t<decltype(shapeA)>;
          using TypeB = std::decay_t<decltype(shapeB)>;

          if constexpr (std::is_same_v<TypeA, CircleShape> &&
                        std::is_same_v<TypeB, CircleShape>) {
            return testCircleCircle(shapeA, shapeB);
          } else if constexpr (std::is_same_v<TypeA, BoxShape> &&
                               std::is_same_v<TypeB, BoxShape>) {
            return testBoxBox(shapeA, shapeB);
          } else if constexpr (std::is_same_v<TypeA, CircleShape> &&
                               std::is_same_v<TypeB, BoxShape>) {
            return testCircleBox(shapeA, shapeB);
          } else if constexpr (std::is_same_v<TypeA, BoxShape> &&
                               std::is_same_v<TypeB, CircleShape>) {
            // Если A — Box, а B — Circle, обращаем нормаль
            Manifold m = testCircleBox(shapeB, shapeA);
            m.normal = -m.normal;
            return m;
          }
        },
        a.shape, b.shape);
  }
};
} // namespace Clicker::Physics
