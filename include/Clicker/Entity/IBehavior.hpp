#pragma once
#include <Clicker/Physics/CollisionSystem.hpp>
#include <SFML/System/Time.hpp>

namespace Clicker {
class Entity;

class IBehavior {
public:
  virtual ~IBehavior() = default;
  virtual void onAttach(Entity &owner) {}
  virtual void update(Entity &owner, sf::Time dt) = 0;
  virtual void onCollision(Entity &owner, Entity &other,
                           const Physics::Manifold &manifold) {}
};
} // namespace Clicker
