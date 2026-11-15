#pragma once

#include "Clicker/Entity/IBehavior.hpp"
#include "Clicker/Physics/Collider.hpp"
#include <Clicker/Entity/EntityComponents.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <optional>
#include <utility>

namespace Clicker {

enum class EntityTag { Default, Player, Enemy, Projectile, Wall, Trigger };

class Entity : public sf::Drawable, public sf::Transformable {
public:
  EntityTag tag{EntityTag::Default};
  bool isAlive{true};

  std::optional<sf::Sprite> sprite;

  std::optional<Physics::Collider> collider;
  std::optional<HealthComponent> health;
  std::optional<MovementComponent> movement;
  std::optional<AnimationComponent> animation;
  std::unique_ptr<IBehavior> behavior;

  Entity() = default;
  explicit Entity(EntityTag entityTag) : tag(entityTag) {}

  void setBehavior(std::unique_ptr<IBehavior> newBehavior) {
    this->behavior = std::move(newBehavior);
    if (this->behavior) {
      this->behavior->onAttach(*this);
    }
  }

  void syncCollider() {
    if (!this->collider.has_value()) {
      return;
    }

    const sf::Vector2f pos = this->getPosition();

    std::visit(
        [&pos](auto &shape) {
          using T = std::decay_t<decltype(shape)>;
          if constexpr (std::is_same_v<T, Physics::CircleShape>) {
            shape.center = pos;
          } else if constexpr (std::is_same_v<T, Physics::BoxShape>) {
            shape.rect.position = pos - (shape.rect.size / 2.0f);
          }
        },
        this->collider->shape);
  }

  void update(sf::Time dt) {
    if (!this->isAlive) {
      return;
    }

    if (this->behavior) {
      this->behavior->update(*this, dt);
    }

    if (this->movement.has_value()) {
      this->movement->update(dt);
      this->move(this->movement->velocity * dt.asSeconds());
    }

    if (this->health.has_value()) {
      this->health->update(dt);
    }

    if (this->animation.has_value() && this->sprite.has_value()) {
      this->animation->update(dt, *this->sprite);
    }

    this->syncCollider();
  }

  void onCollision(Entity &other, const Physics::Manifold &manifold) {
    if (!this->isAlive) {
      return;
    }

    if (this->collider.has_value() && other.collider.has_value()) {
      if (this->collider->type == Physics::CollisionType::Solid &&
          other.collider->type == Physics::CollisionType::Solid) {
        this->move(-manifold.normal * manifold.penetration);
        this->syncCollider();
      }
    }

    if (this->behavior) {
      this->behavior->onCollision(*this, other, manifold);
    }
  }

protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    if (!this->isAlive) {
      return;
    }

    states.transform *= this->getTransform();

    if (this->sprite.has_value()) {
      target.draw(*this->sprite, states);
    }
  }
};

} // namespace Clicker
