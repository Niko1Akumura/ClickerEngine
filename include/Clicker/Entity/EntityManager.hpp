#pragma once

#include "Clicker/Entity/Entity.hpp"
#include "Clicker/Physics/CollisionSystem.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <algorithm>
#include <memory>
#include <vector>

namespace Clicker::Entities {
class EntityManager {
private:
  std::vector<std::unique_ptr<Entity>> m_entities;
  std::vector<std::unique_ptr<Entity>> m_pendingEntities;
  Physics::CollisionSystem m_collisionSystem;

  void flushPendingEntities() {
    if (this->m_pendingEntities.empty()) {
      return;
    }

    for (auto &pending : this->m_pendingEntities) {
      this->m_entities.push_back(std::move(pending));
    }
    this->m_pendingEntities.clear();
  }

  void cleanupDeadEntities() {
    std::erase_if(this->m_entities, [](const std::unique_ptr<Entity> &entity) {
      return !entity || !entity->isAlive;
    });
  }

public:
  template <typename... Args> Entity &spawn(Args &&...args) {
    auto entity = std::make_unique<Entity>(std::forward<Args>(args)...);
    Entity &ref = *entity;
    this->m_pendingEntities.push_back(std::move(entity));
    return ref;
  }

  void update(sf::Time dt) {
    this->flushPendingEntities();

    for (auto &entity : this->m_entities) {
      if (entity && entity->isAlive) {
        entity->update(dt);
      }
    }

    [[maybe_unused]] auto collisions =
        this->m_collisionSystem.update(this->m_entities);

    this->flushPendingEntities();

    this->cleanupDeadEntities();
  }

  void render(sf::RenderTarget &target) const {
    for (const auto &entity : this->m_entities) {
      if (entity && entity->isAlive) {
        target.draw(*entity);
      }
    }
  }

  [[nodiscard]] Entity *findByTag(EntityTag tag) const {
    for (const auto &entity : this->m_entities) {
      if (entity && entity->isAlive && entity->tag == tag) {
        return entity.get();
      }
    }
    return nullptr;
  }

  void clear() {
    this->m_entities.clear();
    this->m_pendingEntities.clear();
  }
};

} // namespace Clicker::Entities
