#pragma once

#include <Clicker/Physics/Common.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <variant>

namespace Clicker {

class Entity;
}

namespace Clicker::Physics {
struct BoxShape {
  sf::FloatRect rect;
};

struct CircleShape {
  sf::Vector2f center;
  float radius{0.0f};
};

using ColliderShape = std::variant<BoxShape, CircleShape>;

class Collider {
public:
  ColliderShape shape;
  CollisionType type{CollisionType::Solid};
  Layer layer{Layer::Default};
  Layer mask{Layer::All};

  Entity *owner{nullptr};

  Collider() = default;

  Collider(ColliderShape shape, CollisionType type, Layer layer, Layer mask)
      : shape(shape), type(type), layer(layer), mask(mask) {}

  [[nodiscard]] bool canCollideWith(const Collider &other) const {
    return ((this->mask & other.layer) != Layer::None) &&
           ((other.mask & this->layer) != Layer::None);
  }

  [[nodiscard]] sf::FloatRect getAABB() const {
    return std::visit(
        [](const auto &s) -> sf::FloatRect {
          using T = std::decay_t<decltype(s)>;
          if constexpr (std::is_same_v<T, BoxShape>) {
            return s.rect;
          } else if constexpr (std::is_same_v<T, CircleShape>) {
            return sf::FloatRect({s.center.x - s.radius, s.center.y - s.radius},
                                 {s.radius * 2.0f, s.radius * 2.0f});
          }
        },
        shape);
  }
};
} // namespace Clicker::Physics
