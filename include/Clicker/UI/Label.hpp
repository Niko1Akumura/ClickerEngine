#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace Clicker::UI {

enum class Alignment { Left, Center, Right };

class Label : public sf::Drawable, public sf::Transformable {
private:
  sf::Text m_text;
  sf::RectangleShape m_background;

  Alignment m_alignment{Alignment::Left};
  float m_padding{5.0f};

  void updateLayout();

public:
  explicit Label(const sf::Font &font, std::string_view text = "",
                 unsigned int characterSize = 24,
                 sf::Color textColor = sf::Color::White,
                 sf::Color backColor = sf::Color::Transparent,
                 sf::Color borderColor = sf::Color::Transparent,
                 float borderWidth = 0.0f);

  void setText(std::string_view text);
  void setCharacterSize(unsigned int size);
  void setAlignment(Alignment alignment);
  void setPadding(float padding);

  void setTextColor(sf::Color color);
  void setBackgroundColor(sf::Color color);
  void setBorder(sf::Color color, float thickness);

  sf::FloatRect getGlobalBounds() const;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
} // namespace Clicker::UI
