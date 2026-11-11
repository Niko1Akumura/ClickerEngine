#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>

namespace Clicker::Physics {

enum Layer : uint32_t {
  None = 0,
  Default = 1 << 0,      // 1  (0000 0001)
  World = 1 << 1,        // 2  (Стены, границы карты)
  Player = 1 << 2,       // 4  (Сам игрок)
  Enemy = 1 << 3,        // 8  (Враги)
  PlayerAttack = 1 << 4, // 16 (Удар меча, пули игрока)
  EnemyAttack = 1 << 5,  // 32 (Снаряды врагов)
  Collectible = 1 << 6,  // 64 (Монетки, лут)
  All = 0xFFFFFFFF
};

enum class CollisionType { Solid, Trigger };

struct Manifold {
  bool hasCollision{false};
  sf::Vector2f normal{0.0f, 0.0f};
  float penetration{0.0f};
};

inline Layer operator|(Layer a, Layer b) {
  return static_cast<Layer>(static_cast<uint32_t>(a) |
                            static_cast<uint32_t>(b));
}

inline Layer operator&(Layer a, Layer b) {
  return static_cast<Layer>(static_cast<uint32_t>(a) &
                            static_cast<uint32_t>(b));
}

} // namespace Clicker::Physics
