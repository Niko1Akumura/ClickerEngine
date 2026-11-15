#pragma once

#include <Clicker/Entity/EntityManager.hpp>
#include <Clicker/ResourceManager/Resources.hpp>
#include <Clicker/Scene/IScene.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

namespace Clicker::Scene {

class TestSceene : public Clicker::Scene::IScene {
private:
  Clicker::Entities::EntityManager m_manager;
  sf::Texture m_placeholderTexture;
  void initRes();
  void initEntity();

public:
  explicit TestSceene(Resource::Context context);

  void handleEvent(sf::RenderWindow &window) override;
  void update(sf::Time dt) override;
  void render(sf::RenderTarget &targer) override;
};
} // namespace Clicker::Scene
