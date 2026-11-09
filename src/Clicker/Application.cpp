#include <Clicker/Application.hpp>
#include <SFML/Graphics/Color.hpp>
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

Application::Application(sf::Vector2u screenSize, const std::string &title,
                         float a)
    : m_window(sf::VideoMode(screenSize), title), m_circle(a) {
  this->initRes();
  this->m_circle.setTexture(&this->m_textures.get("main"));
  this->m_circle.setPosition(sf::Vector2f({200, 100}));
}

void Application::handlerEvent() {
  const auto onClose = [this](const sf::Event::Closed &) {
    this->m_window.close();
  };
  this->m_window.handleEvents(onClose);
}

void Application::update(sf::Time dt) {
  this->m_circle.move(dt, this->m_window.getSize().x);
  std::cout << std::to_string(this->m_circle.getPosition());
}

void Application::draw() {
  this->m_window.clear();
  this->m_window.draw(this->m_circle);
  this->m_window.display();
}

void Application::initRes() {
  this->m_textures.loadResource(
      "main",
      "assets/textures/128x128/Circular/Brick/Circular_Brick_02-128x128.png",
      true);
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
