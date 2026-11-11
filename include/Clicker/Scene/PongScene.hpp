#pragma once

#include "Clicker/ResourceManager/Resources.hpp"
#include <Clicker/Entity/Circle.hpp>
#include <Clicker/Physics/CollisionSystem.hpp>
#include <Clicker/Scene/IScene.hpp>
#include <Clicker/UI/Label.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <optional>

namespace Clicker::Scene {
class PongScene : public IScene {
private:
  sf::RectangleShape m_paddle;
  sf::RectangleShape m_paddleL;
  Entity::Circle m_circle;

  Physics::CollisionSystem m_physicsSystem;
  Physics::Collider m_ballCollider;
  Physics::Collider m_paddleCollider;
  Physics::Collider m_paddleLCollider;

  std::optional<sf::Sound> m_sound;
  std::optional<UI::Label> m_label;

  void initEntity();
  void updateColliders();

  int m_count = 0;
  int m_score = 0;

public:
  explicit PongScene(Resource::Context context);

  void handleEvent(sf::RenderWindow &window) override;
  void update(sf::Time dt) override;
  void render(sf::RenderTarget &targer) override;
};
} // namespace Clicker::Scene
