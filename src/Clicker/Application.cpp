#include "Clicker/Entity/Circle.hpp"
#include <Clicker/Application.hpp>
#include <Clicker/Collision/Collision.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <string>
namespace Clicker {

Application::Application(sf::Vector2u screenSize, const std::string &title)
    : m_window(sf::VideoMode(screenSize), title) {
  this->initRes();
  this->initEntity();
}

void Application::handlerEvent() {
  const auto onClose = [this](const sf::Event::Closed &) {
    this->m_window.close();
  };
  this->m_window.handleEvents(onClose);
}

void Application::update(sf::Time dt) {
  bool hitRight =
      Collision::Collision::hasCollisoin(this->m_circle, this->m_paddle);
  bool hitLeft =
      Collision::Collision::hasCollisoin(this->m_circle, this->m_paddleL);

  if (hitRight || hitLeft) {
    count++;
    if (count % 2 == 0 && count <= 6) {
      this->m_circle.setSpeed(this->m_circle.getSpeed() * 1.5f);
    }
    this->m_circle.reverse();
  }

  this->m_circle.move(dt);
}

void Application::draw() {
  this->m_window.clear();
  this->m_window.draw(this->m_circle);
  this->m_window.draw(this->m_paddle);
  this->m_window.draw(this->m_paddleL);
  this->m_window.display();
}

void Application::initRes() {
  this->m_textures.loadResource(
      "main",
      "assets/textures/128x128/Circular/Brick/Circular_Brick_02-128x128.png",
      true);
  this->m_textures.loadResource(
      "paddle",
      "assets/textures/128x128/Square/Wood/Square_Wood_01-128x128.png", true);
}

void Application::initEntity() {
  this->m_circle = Entity::Circle(20.0f);
  this->m_circle.setTexture(&this->m_textures.get("main"));
  this->m_circle.setPosition(sf::Vector2f(
      {this->m_circle.getRadius() * 2.0f, this->m_window.getSize().y / 2.0f}));

  this->m_paddle = sf::RectangleShape(sf::Vector2f({10.0f, 200.0f}));
  this->m_paddle.setOrigin(this->m_paddle.getSize() / 2.0f);
  this->m_paddle.setPosition(
      sf::Vector2f({this->m_window.getSize().x - this->m_paddle.getSize().x,
                    this->m_window.getSize().y / 2.f}));
  this->m_paddle.setTexture(&this->m_textures.get("paddle"));
  this->m_paddleL = sf::RectangleShape(sf::Vector2f({10.0f, 200.0f}));
  this->m_paddleL.setOrigin(this->m_paddle.getSize() / 2.0f);
  this->m_paddleL.setPosition(sf::Vector2f(
      {this->m_paddle.getSize().x, this->m_window.getSize().y / 2.f}));
  this->m_paddleL.setTexture(&this->m_textures.get("paddle"));
}

void Application::run() {
  this->m_window.setFramerateLimit(60);
  while (this->m_window.isOpen()) {
    sf::Time dt = this->m_clock.restart();
    this->handlerEvent();
    this->update(dt);
    this->draw();
  }
};

} // namespace Clicker
