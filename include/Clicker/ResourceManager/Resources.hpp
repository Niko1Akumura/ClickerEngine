#pragma once
#include <Clicker/ResourceManager/ResourceManager.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Clicker::Resource {
enum TFont { Main = 0 };

using Fonts = Clicker::Resource::ResourceManager<sf::Font, std::string>;
using Textures = Clicker::Resource::ResourceManager<sf::Texture, std::string>;
using Sounds = Clicker::Resource::ResourceManager<sf::SoundBuffer, std::string>;

struct Context {
  Resource::Fonts &m_fonts;
  Resource::Textures &m_textures;
  Resource::Sounds &m_sounds;
  sf::RenderWindow &m_window;
};
} // namespace Clicker::Resource
