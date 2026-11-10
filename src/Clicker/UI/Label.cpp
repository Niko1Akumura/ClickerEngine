#include <Clicker/UI/Label.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
namespace Clicker::UI {
Label::Label(const sf::Font &font, std::string_view text,
             unsigned int characterSize, sf::Color textColor,
             sf::Color backColor, sf::Color borderColor, float borderWidth)
    : m_text(font, sf::String::fromUtf8(text.begin(), text.end()),
             characterSize) {
  m_text.setFillColor(textColor);

  m_background.setFillColor(backColor);
  m_background.setOutlineColor(borderColor);
  m_background.setOutlineThickness(borderWidth);

  updateLayout();
}

void Label::updateLayout() {
  sf::FloatRect textBounds = m_text.getLocalBounds();

  float width = textBounds.size.x + m_padding * 2.0f;
  float height = textBounds.size.y + m_padding * 2.0f;
  m_background.setSize({width, height});

  float textX = m_padding - textBounds.position.x;
  if (m_alignment == Alignment::Center) {
    textX = (width - textBounds.size.x) / 2.0f - textBounds.position.x;
  } else if (m_alignment == Alignment::Right) {
    textX = width - textBounds.size.x - m_padding - textBounds.position.x;
  }

  float textY = m_padding - textBounds.position.y;
  m_text.setPosition({textX, textY});
}

void Label::setText(std::string_view text) {
  this->m_text.setString(text);
  this->updateLayout();
}

void Label::setCharacterSize(unsigned int size) {
  this->m_text.setCharacterSize(size);
  this->updateLayout();
}

void Label::setPadding(float padding) {
  this->m_padding = padding;
  this->updateLayout();
}

void Label::setTextColor(sf::Color color) { this->m_text.setFillColor(color); }

void Label::setBackgroundColor(sf::Color color) {
  this->m_background.setFillColor(color);
}

void Label::setBorder(sf::Color color, float thickness) {
  this->m_background.setOutlineThickness(thickness);
  this->m_background.setOutlineColor(color);
}

sf::FloatRect Label::getGlobalBounds() const {
  return this->getTransform().transformRect(
      this->m_background.getLocalBounds());
}

void Label::setAlignment(UI::Alignment alignment) {
  if (this->m_alignment != alignment) {
    this->m_alignment = alignment;
    this->updateLayout();
  }
}

void Label::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= this->getTransform();

  if (this->m_background.getSize().x > 0.0f &&
      this->m_background.getSize().y > 0.0f) {
    target.draw(this->m_background, states);
  }
  target.draw(this->m_text, states);
}
} // namespace Clicker::UI
