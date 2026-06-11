#include "UIInventoryMenu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

UIInventoryMenu::UIInventoryMenu() : m_position(0.f, 0.f), m_size(0.f, 0.f) {}

UIInventoryMenu::UIInventoryMenu(sf::Vector2f resolution, const sf::Font &font)
    : m_font(font) {
  m_size = sf::Vector2f(640.f, 480.f);
  m_position = sf::Vector2f((resolution.x - m_size.x) / 2.f,
                            (resolution.y - m_size.y) / 2.f);

  m_bgRect.setPosition(m_position);
  m_bgRect.setSize(m_size);
  m_bgRect.setFillColor(sf::Color(10, 15, 20, 235));

  m_borderRect.setPosition(m_position);
  m_borderRect.setSize(m_size);
  m_borderRect.setFillColor(sf::Color::Transparent);
  m_borderRect.setOutlineThickness(2.0f);
  m_borderRect.setOutlineColor(
      sf::Color(138, 43, 226, 180)); // Purple outline for inventory

  m_titleText.setFont(m_font);
  m_titleText.setString("INWENTARZ STATKU");
  m_titleText.setCharacterSize(26);
  m_titleText.setFillColor(sf::Color(138, 43, 226));
  m_titleText.setStyle(sf::Text::Bold);
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(1.5f);
  centerTextX(m_titleText, m_position.y + 40.f);

  auto initText = [&](sf::Text &t) {
    t.setFont(m_font);
    t.setCharacterSize(18);
    t.setStyle(sf::Text::Bold);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(1.f);
    t.setFillColor(sf::Color::White);
  };

  initText(m_scrapText);
  initText(m_electronicsText);
  initText(m_rareMetalsText);

  m_instructionsText.setFont(m_font);
  m_instructionsText.setString("Nacisnij [K] lub [Esc] aby zamknac panel");
  m_instructionsText.setCharacterSize(14);
  m_instructionsText.setFillColor(sf::Color(150, 160, 175));
  m_instructionsText.setOutlineColor(sf::Color::Black);
  m_instructionsText.setOutlineThickness(1.f);
  centerTextX(m_instructionsText, m_position.y + m_size.y - 40.f);
}

void UIInventoryMenu::centerTextX(sf::Text &text, float y) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.left + bounds.width / 2.0f,
                 bounds.top + bounds.height / 2.0f);
  text.setPosition(m_position.x + m_size.x / 2.0f, y);
}

void UIInventoryMenu::update(Ship &ship) {
  std::stringstream ssScrap, ssElec, ssRare;

  ssScrap << "Zlom: " << static_cast<int>(ship.getCurrentScrap()) << " / "
          << static_cast<int>(ship.getMaxScrap());
  m_scrapText.setString(ssScrap.str());

  ssElec << "Elektronika: " << static_cast<int>(ship.getCurrentElectronics())
         << " / " << static_cast<int>(ship.getMaxElectronics());
  m_electronicsText.setString(ssElec.str());

  ssRare << "Rzadkie Metale: " << static_cast<int>(ship.getCurrentRareMetals())
         << " / " << static_cast<int>(ship.getMaxRareMetals());
  m_rareMetalsText.setString(ssRare.str());

  centerTextX(m_scrapText, m_position.y + 120.f);
  centerTextX(m_electronicsText, m_position.y + 180.f);
  centerTextX(m_rareMetalsText, m_position.y + 240.f);
}

void UIInventoryMenu::draw(sf::RenderWindow &window) {
  window.draw(m_bgRect);
  window.draw(m_borderRect);
  window.draw(m_titleText);
  window.draw(m_scrapText);
  window.draw(m_electronicsText);
  window.draw(m_rareMetalsText);
  window.draw(m_instructionsText);
}
