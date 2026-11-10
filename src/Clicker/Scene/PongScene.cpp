#include "Clicker/Scene/PongScene.hpp"
#include "Clicker/ResourceManager/Resources.hpp"
#include <Clicker/Collision/Collision.hpp>
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
  bool hitRight =
      Collision::Collision::hasCollisoin(this->m_circle, this->m_paddle);
  bool hitLeft =
      Collision::Collision::hasCollisoin(this->m_circle, this->m_paddleL);

  if (hitRight || hitLeft) {
    if (this->m_sound) {
      this->m_sound->play();
    }
    this->score++;
    count++;
    if (this->m_label) {
      this->m_label->setText("Score: " + std::to_string(this->score));
    }

    if (count % 2 == 0 && count <= 6) {
      this->m_circle.setSpeed(this->m_circle.getSpeed() * 1.5f);
    }
    this->m_circle.reverse();
  }

  this->m_circle.move(dt);
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
}

} // namespace Clicker::Scene
