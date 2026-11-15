#pragma once

#include "Clicker/Entity/Entity.hpp"
#include "Clicker/Entity/EntityManager.hpp"
#include "Clicker/Entity/IBehavior.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

namespace Clicker::Entities {

class MeleeHitboxBehavior : public IBehavior {
private:
  float m_lifetime{0.15f};
  float m_damage{25.0f};
  EntityTag m_targetTag{EntityTag::Enemy};

public:
  MeleeHitboxBehavior(float damage, float lifetime = 0.15f,
                      EntityTag targetTag = EntityTag::Enemy)
      : m_lifetime(lifetime), m_damage(damage), m_targetTag(targetTag) {}

  void update(Entity &owner, sf::Time dt) override {
    this->m_lifetime -= dt.asSeconds();
    if (this->m_lifetime <= 0.0f) {
      owner.isAlive = false;
    }
  }

  void onCollision(Entity &owner, Entity &other,
                   const Physics::Manifold & /*manifold*/) override {
    if (other.tag == this->m_targetTag && other.health.has_value()) {
      other.health->takeDamage(this->m_damage);
    }
  }
};

class PlayerBehavior : public IBehavior {
private:
  EntityManager &m_entityManager;
  sf::Vector2f m_facingDir{1.0f, 0.0f};
  float m_attackCooldown{0.4f};
  float m_attackTimer{0.0f};

  void spawnSwordSlash(Entity &owner) {
    Entity &slash = this->m_entityManager.spawn(EntityTag::Trigger);
    slash.setPosition(owner.getPosition() + this->m_facingDir * 40.0f);

    slash.collider = Physics::Collider(
        Physics::BoxShape{sf::FloatRect({0.0f, 0.0f}, {50.0f, 50.0f})},
        Physics::CollisionType::Trigger, Physics::Layer::PlayerAttack,
        Physics::Layer::Enemy);
    slash.syncCollider();
    slash.setBehavior(
        std::make_unique<MeleeHitboxBehavior>(25.0f, 0.15f, EntityTag::Enemy));
  }

public:
  explicit PlayerBehavior(EntityManager &entityManager)
      : m_entityManager(entityManager) {}

  void update(Entity &owner, sf::Time dt) override {
    if (this->m_attackTimer > 0.0f) {
      this->m_attackTimer -= dt.asSeconds();
    }

    sf::Vector2f inputDir{0.0f, 0.0f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
      inputDir.y -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      inputDir.y += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
      inputDir.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      inputDir.x += 1.0f;
    }

    float length = std::sqrt(inputDir.x * inputDir.x + inputDir.y * inputDir.y);
    if (length > 0.0f) {
      inputDir /= length;
      this->m_facingDir = inputDir;
    }

    if (owner.movement.has_value()) {
      owner.movement->velocity = inputDir * owner.movement->maxSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
        this->m_attackTimer <= 0.0f) {
      this->spawnSwordSlash(owner);
      this->m_attackTimer = this->m_attackCooldown;
    }
  }
};

class ChaseEnemyBehavior : public IBehavior {
private:
  EntityManager &m_entityManager;
  float m_contactDamage{10.0f};

public:
  explicit ChaseEnemyBehavior(EntityManager &entityManager,
                              float contactDamage = 10.0f)
      : m_entityManager(entityManager), m_contactDamage(contactDamage) {}

  void update(Entity &owner, sf::Time /*dt*/) override {
    if (!owner.movement.has_value()) {
      return;
    }

    Entity *target = this->m_entityManager.findByTag(EntityTag::Player);
    if (!target) {
      owner.movement->velocity = {0.0f, 0.0f};
      return;
    }

    sf::Vector2f dir = target->getPosition() - owner.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist > 5.0f) {
      owner.movement->velocity = (dir / dist) * owner.movement->maxSpeed;
    } else {
      owner.movement->velocity = {0.0f, 0.0f};
    }
  }

  void onCollision(Entity & /*owner*/, Entity &other,
                   const Physics::Manifold & /*manifold*/) override {
    if (other.tag == EntityTag::Player && other.health.has_value()) {
      other.health->takeDamage(this->m_contactDamage);
    }
  }
};

} // namespace Clicker::Entities
