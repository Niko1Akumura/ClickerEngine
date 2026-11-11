#include "Clicker/Scene/PongScene.hpp"
#include "Clicker/ResourceManager/Resources.hpp"
#include <Clicker/Scene/IScene.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
namespace Clicker::Scene {

PongScene::PongScene(Resource::Context context) : IScene(context) {
  this->m_sound.emplace(this->m_context.m_sounds.get("pong_coll"));
  this->initEntity();
}

void PongScene::update(sf::Time dt) {
  // 1. Двигаем шар
  this->m_circle.move(dt);

  // 2. Синхронизируем коллайдеры с новыми координатами
  this->updateColliders();

  // 3. Прогоняем Broad Phase + Narrow Phase
  auto collisions = this->m_physicsSystem.update();

  for (const auto &pair : collisions) {
    // Проверяем, участвует ли шар в столкновении
    if (pair.first == &this->m_ballCollider ||
        pair.second == &this->m_ballCollider) {

      // Выталкиваем шар из ракетки, чтобы он не застрял внутри
      // Направление нормали всегда идёт от first к second
      sf::Vector2f resolveDir = (pair.first == &this->m_ballCollider)
                                    ? -pair.manifold.normal
                                    : pair.manifold.normal;

      this->m_circle.setPosition(this->m_circle.getPosition() +
                                 resolveDir * pair.manifold.penetration);

      // Реакция: разворот скорости и звук
      this->m_score++;
      if (this->m_label) {
        this->m_label->setText("Score: " + std::to_string(this->m_score));
      }

      if (this->m_score % 2 == 0 && this->m_score <= 6) {
        this->m_circle.setSpeed(this->m_circle.getSpeed() * 1.5f);
      }

      this->m_circle.reverse();
      this->m_sound->play();

      // Синхронизируем коллайдер после выталкивания
      this->updateColliders();
    }
  }
}
void PongScene::handleEvent(sf::RenderWindow &window) {}
void PongScene::render(sf::RenderTarget &targer) {
  targer.draw(this->m_circle);
  targer.draw(this->m_paddle);
  targer.draw(this->m_paddleL);
  if (this->m_label) {
    targer.draw(*this->m_label);
  }
}

void PongScene::initEntity() {
  // Label
  this->m_label.emplace(this->m_context.m_fonts.get("main"), "Score: 0",
                        28,                      // размер шрифта
                        sf::Color::White,        // цвет текста
                        sf::Color(0, 0, 0, 150), // полупрозрачный черный фон
                        sf::Color::White,        // цвет рамки
                        2.0f);

  this->m_label->setPadding(10.0f);
  this->m_label->setAlignment(UI::Alignment::Center);

  // Центрируем плашку по горизонтали вверху окна
  float winWidth = static_cast<float>(this->m_context.m_window.getSize().x);
  float labelWidth = this->m_label->getGlobalBounds().size.x;

  this->m_label->setPosition({(winWidth - labelWidth) / 2.0f, 20.0f});
  // Ball
  this->m_circle = Entity::Circle(20.0f);
  this->m_circle.setTexture(&this->m_context.m_textures.get("main"));
  this->m_circle.setPosition(
      sf::Vector2f({this->m_circle.getRadius() * 2.0f,
                    this->m_context.m_window.getSize().y / 2.0f}));
  // Right paddle
  this->m_paddle = sf::RectangleShape(sf::Vector2f({10.0f, 200.0f}));
  this->m_paddle.setOrigin(this->m_paddle.getSize() / 2.0f);
  this->m_paddle.setPosition(sf::Vector2f(
      {this->m_context.m_window.getSize().x - this->m_paddle.getSize().x,
       this->m_context.m_window.getSize().y / 2.f}));
  this->m_paddle.setTexture(&this->m_context.m_textures.get("paddle"));
  // Left paddle
  this->m_paddleL = sf::RectangleShape(sf::Vector2f({10.0f, 200.0f}));
  this->m_paddleL.setOrigin(this->m_paddle.getSize() / 2.0f);
  this->m_paddleL.setPosition(
      sf::Vector2f({this->m_paddle.getSize().x,
                    this->m_context.m_window.getSize().y / 2.f}));
  this->m_paddleL.setTexture(&this->m_context.m_textures.get("paddle"));
  // Physics
  m_ballCollider = Physics::Collider(
      Physics::CircleShape{m_circle.getPosition(), m_circle.getRadius()},
      Physics::CollisionType::Solid, Physics::Layer::Player,
      Physics::Layer::World);

  auto paddleBox = [](const sf::RectangleShape &rect) {
    return Physics::BoxShape{rect.getGlobalBounds()};
  };

  m_paddleCollider =
      Physics::Collider(paddleBox(m_paddle), Physics::CollisionType::Solid,
                        Physics::Layer::World, Physics::Layer::Player);

  m_paddleLCollider =
      Physics::Collider(paddleBox(m_paddleL), Physics::CollisionType::Solid,
                        Physics::Layer::World, Physics::Layer::Player);

  m_physicsSystem.clear();
  m_physicsSystem.addCollider(&m_ballCollider);
  m_physicsSystem.addCollider(&m_paddleCollider);
  m_physicsSystem.addCollider(&m_paddleLCollider);
}

void PongScene::updateColliders() {
  sf::Vector2f circleCenter =
      m_circle.getPosition() +
      sf::Vector2f(m_circle.getRadius(), m_circle.getRadius());
  // updateColliders():
  m_ballCollider.shape =
      Physics::CircleShape{m_circle.getPosition(), m_circle.getRadius()};
  // Обновляем AABB ракеток
  m_paddleCollider.shape = Physics::BoxShape{m_paddle.getGlobalBounds()};
  m_paddleLCollider.shape = Physics::BoxShape{m_paddleL.getGlobalBounds()};
}

} // namespace Clicker::Scene
