#include <Clicker/Entity/Behaviors.hpp>
#include <Clicker/Entity/Entity.hpp>
#include <Clicker/Scene/TestScene.hpp>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

namespace Clicker::Scene {

TestSceene::TestSceene(Resource::Context context)
    : Clicker::Scene::IScene(context) {
  this->initRes();
  this->initEntity();
}

void TestSceene::initRes() {
  sf::Image img({40, 40}, sf::Color::White);
  if (!this->m_placeholderTexture.loadFromImage(img)) {
    std::cerr << "Failed to create placeholder texture\n";
  }
}

void TestSceene::initEntity() {
  auto &player = this->m_manager.spawn(EntityTag::Player);
  player.setPosition({300.0f, 300.0f});

  player.sprite.emplace(this->m_placeholderTexture);
  player.sprite->setColor(sf::Color::Green);
  player.sprite->setOrigin({20.0f, 20.0f});

  player.movement = MovementComponent{.maxSpeed = 250.0f};
  player.health = HealthComponent(100.0f);
  player.health->onDamageTaken = [](float dmg) {
    std::cout << "Player took damage: " << dmg << "\n";
  };

  player.collider = Physics::Collider(
      Physics::BoxShape{sf::FloatRect({0.0f, 0.0f}, {40.0f, 40.0f})},
      Physics::CollisionType::Solid, Physics::Layer::Player,
      Physics::Layer::World | Physics::Layer::Enemy);
  player.syncCollider();
  player.setBehavior(
      std::make_unique<Entities::PlayerBehavior>(this->m_manager));
  player.health->onDeath = [&player]() {
    std::cout << "Player died!\n";
    player.sprite->setColor(
        sf::Color(80, 80, 80)); // или player.isAlive = false;
  };
  auto &enemy = this->m_manager.spawn(EntityTag::Enemy);
  enemy.setPosition({700.0f, 300.0f});

  enemy.sprite.emplace(this->m_placeholderTexture);
  enemy.sprite->setColor(sf::Color::Red);
  enemy.sprite->setOrigin({20.0f, 20.0f});

  enemy.movement = MovementComponent{.maxSpeed = 130.0f};
  enemy.health = HealthComponent(50.0f);
  enemy.health->onDamageTaken = [&enemy](float dmg) {
    std::cout << "Enemy took damage: " << dmg
              << " (HP left: " << enemy.health->currentHp << ")\n";
  };
  enemy.health->onDeath = [&enemy]() {
    std::cout << "Enemy defeated!\n";
    enemy.isAlive = false;
  };

  enemy.collider = Physics::Collider(
      Physics::BoxShape{sf::FloatRect({0.0f, 0.0f}, {40.0f, 40.0f})},
      Physics::CollisionType::Solid, Physics::Layer::Enemy,
      Physics::Layer::World | Physics::Layer::Player |
          Physics::Layer::PlayerAttack);
  enemy.syncCollider();
  enemy.setBehavior(
      std::make_unique<Entities::ChaseEnemyBehavior>(this->m_manager, 15.0f));

  auto &wall = this->m_manager.spawn(EntityTag::Wall);
  wall.setPosition({500.0f, 250.0f});

  wall.sprite.emplace(this->m_placeholderTexture);
  wall.sprite->setColor(sf::Color(120, 120, 120));
  wall.sprite->setScale({1.0f, 4.0f}); // Растягиваем в стенку 40x160
  wall.sprite->setOrigin({20.0f, 20.0f});

  wall.collider = Physics::Collider(
      Physics::BoxShape{sf::FloatRect({0.0f, 0.0f}, {40.0f, 160.0f})},
      Physics::CollisionType::Solid, Physics::Layer::World,
      Physics::Layer::Player | Physics::Layer::Enemy);
  wall.syncCollider();
}

void TestSceene::update(sf::Time dt) { this->m_manager.update(dt); }
void TestSceene::handleEvent(sf::RenderWindow & /*window*/) {}

void TestSceene::render(sf::RenderTarget &target) {
  this->m_manager.render(target);
}

} // namespace Clicker::Scene
