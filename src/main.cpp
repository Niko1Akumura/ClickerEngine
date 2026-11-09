#include <Clicker/Application.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

int main() {
  std::unique_ptr<Clicker::Application> app =
      std::make_unique<Clicker::Application>(sf::Vector2u({800, 600}), "SFML");
  app->run();
}
