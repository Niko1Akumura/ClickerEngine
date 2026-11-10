#pragma once

#include <Clicker/ResourceManager/Resources.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Clicker::Scene {
class IScene {
protected:
  Resource::Context m_context;

public:
  explicit IScene(Resource::Context context) : m_context(context) {}
  virtual ~IScene() = default;

  virtual void handleEvent(sf::RenderWindow &window) = 0;
  virtual void update(sf::Time dt) = 0;
  virtual void render(sf::RenderTarget &targer) = 0;
};
} // namespace Clicker::Scene
