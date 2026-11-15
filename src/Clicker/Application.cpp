#include "Clicker/ResourceManager/Resources.hpp"
#include <Clicker/Application.hpp>
#include <Clicker/Scene/TestScene.hpp>
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
#include <memory>
#include <string>
namespace Clicker {

Application::Application(sf::Vector2u screenSize, const std::string &title)
    : m_window(sf::VideoMode(screenSize), title) {
  this->initRes();
  this->initScene();
}

void Application::handlerEvent() {
  const auto onClose = [this](const sf::Event::Closed &) {
    this->m_window.close();
  };
  this->m_window.handleEvents(onClose);
}

void Application::update(sf::Time dt) {
  if (this->m_scene) {
    this->m_scene->update(dt);
  }
}

void Application::draw() {
  this->m_window.clear();
  if (this->m_scene) {
    this->m_scene->render(this->m_window);
  }
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
  this->m_sounds.loadResource("pong_coll", "assets/sounds/pong/pong_coll.mp3");
  this->m_fonts.loadResource("main", "assets/fonts/DejaVuSans-Bold.ttf");
}

void Application::initScene() {
  this->m_scene = std::make_unique<Scene::TestSceene>(Resource::Context{
      this->m_fonts, this->m_textures, this->m_sounds, this->m_window});
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
