#pragma once

#include "Clicker/Physics/Common.hpp"
#include <Clicker/Physics/Collider.hpp>
#include <Clicker/Physics/NarrowPhase.hpp>
#include <memory>
#include <vector>

namespace Clicker::Physics {
struct CollisionPair {
  Collider *first{nullptr};
  Collider *second{nullptr};
  Manifold manifold;
};

class CollisionSystem {
private:
  std::vector<Collider *> m_colliders;

public:
  void addCollider(Collider *collider) {
    if (collider) {
      this->m_colliders.push_back(collider);
    }
  }

  void clear() { this->m_colliders.clear(); }

  template <typename EntityContainer>
  [[nodiscard]] std::vector<CollisionPair> update(EntityContainer &entities) {
    this->m_colliders.clear();
    this->m_colliders.reserve(entities.size());

    for (auto &entityPtr : entities) {
      if (entityPtr && entityPtr->isAlive && entityPtr->collider.has_value()) {
        entityPtr->collider->owner = entityPtr.get();
        this->m_colliders.push_back(&(*entityPtr->collider));
      }
    }

    auto collisions = this->update();

    using EntityType = typename EntityContainer::value_type::element_type;

    for (const auto &pair : collisions) {
      if (pair.first->owner && pair.second->owner) {
        auto *entityA = static_cast<EntityType *>(pair.first->owner);
        auto *entityB = static_cast<EntityType *>(pair.second->owner);

        entityA->onCollision(*entityB, pair.manifold);

        Manifold inverted = pair.manifold;
        inverted.normal = -inverted.normal;
        entityB->onCollision(*entityA, inverted);
      }
    }

    return collisions;
  }

  [[nodiscard]] std::vector<CollisionPair> update() {
    std::vector<CollisionPair> collisions;
    const size_t count = this->m_colliders.size();

    for (size_t i = 0; i < count; ++i) {
      for (size_t j = i + 1; j < count; ++j) {
        Collider *a = this->m_colliders[i];
        Collider *b = this->m_colliders[j];

        // 1. Фильтрация по битовым маскам слоев
        if (!a->canCollideWith(*b)) {
          continue;
        }

        // 2. Broad Phase: проверка пересечения AABB
        sf::FloatRect aabbA = a->getAABB();
        sf::FloatRect aabbB = b->getAABB();

        if (!aabbA.findIntersection(aabbB).has_value()) {
          continue; // AABB не пересекаются, пропускаем точную геометрию
        }

        // 3. Narrow Phase: точный геометрический тест
        Manifold manifold = NarrowPhase::testCollision(*a, *b);
        if (manifold.hasCollision) {
          collisions.push_back({a, b, manifold});
        }
      }
    }

    return collisions;
  }
};
} // namespace Clicker::Physics
