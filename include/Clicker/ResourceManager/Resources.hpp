#pragma once
#include <Clicker/ResourceManager/ResourceManager.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Clicker::Resource {
enum TFont { Main = 0 };

using Fonts = Clicker::Resource::ResourceManager<sf::Font, TFont>;
using Textures = Clicker::Resource::ResourceManager<sf::Texture, std::string>;
using Sounds = Clicker::Resource::ResourceManager<sf::SoundBuffer, std::string>;
} // namespace Clicker::Resource
