#include "UIInventoryBar.h"
#include <iostream>
#include <sstream>
#include <iomanip>

UIInventoryBar::UIInventoryBar() : m_position(0.f, 0.f), m_size(0.f, 0.f) {}

UIInventoryBar::UIInventoryBar(sf::Vector2f position, sf::Vector2f size,
                               const sf::Font &font)
    : m_position(position), m_size(size) {

  m_bgRect.setPosition(m_position);
  m_bgRect.setSize(m_size);
  m_bgRect.setFillColor(sf::Color(15, 15, 22, 200));

  m_borderRect.setPosition(m_position);
  m_borderRect.setSize(m_size);
  m_borderRect.setFillColor(sf::Color::Transparent);
  m_borderRect.setOutlineThickness(1.5f);
  m_borderRect.setOutlineColor(sf::Color(138, 43, 226, 180));

  float padding = 3.f;
  float gap = 2.f;
  float totalInnerWidth = m_size.x - 2.f * padding - 2.f * gap;
  float partWidth = totalInnerWidth / 3.f;
  float innerHeight = m_size.y - 2.f * padding;

  m_scrapFill.setPosition(m_position.x + padding, m_position.y + padding);
  m_scrapFill.setSize(sf::Vector2f(0.f, innerHeight));
  m_scrapFill.setFillColor(sf::Color(220, 160, 30));

  m_elecFill.setPosition(m_position.x + padding + partWidth + gap,
                         m_position.y + padding);
  m_elecFill.setSize(sf::Vector2f(0.f, innerHeight));
  m_elecFill.setFillColor(sf::Color(0, 190, 220));

  m_rareFill.setPosition(m_position.x + padding + 2.f * (partWidth + gap),
                         m_position.y + padding);
  m_rareFill.setSize(sf::Vector2f(0.f, innerHeight));
  m_rareFill.setFillColor(sf::Color(200, 50, 200));

  int charSize = 13;
  m_scrapText.setFont(font);
  m_scrapText.setCharacterSize(charSize);
  m_elecText.setFont(font);
  m_elecText.setCharacterSize(charSize);
  m_rareText.setFont(font);
  m_rareText.setCharacterSize(charSize);

  auto setupText = [](sf::Text &t) {
    t.setFillColor(sf::Color::White);
    t.setStyle(sf::Text::Bold);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(1.f);
  };

    setupText(m_scrapText);
    setupText(m_elecText);
    setupText(m_rareText);
}

void UIInventoryBar::update(float dt, Ship &ship) {
  m_scrapCur = ship.getCurrentScrap();
  m_scrapMax = ship.getMaxScrap();
  m_elecCur = ship.getCurrentElectronics();
  m_elecMax = ship.getMaxElectronics();
  m_rareCur = ship.getCurrentRareMetals();
  m_rareMax = ship.getMaxRareMetals();

  float padding = 3.f;
  float gap = 2.f;
  float totalInnerWidth = m_size.x - 2.f * padding - 2.f * gap;
  float partWidth = totalInnerWidth / 3.f;

  m_scrapFill.setSize(
      sf::Vector2f(partWidth * (m_scrapMax > 0 ? m_scrapCur / m_scrapMax : 0),
                   m_size.y - 2.f * padding));
  m_elecFill.setSize(
      sf::Vector2f(partWidth * (m_elecMax > 0 ? m_elecCur / m_elecMax : 0),
                   m_size.y - 2.f * padding));
  m_rareFill.setSize(
      sf::Vector2f(partWidth * (m_rareMax > 0 ? m_rareCur / m_rareMax : 0),
                   m_size.y - 2.f * padding));

  auto updateText = [&](sf::Text &t, float cur, float startX,
                        const std::string &prefix) {
    t.setString(prefix + std::to_string((int)cur));
    sf::FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.left + bounds.width / 2.f,
                bounds.top + bounds.height / 2.f);
    t.setPosition(startX + partWidth / 2.f, m_position.y + m_size.y / 2.f);
  };

  updateText(m_scrapText, m_scrapCur, m_position.x + padding, "Z: ");
  updateText(m_elecText, m_elecCur, m_position.x + padding + partWidth + gap,
             "E: ");
  updateText(m_rareText, m_rareCur,
             m_position.x + padding + 2.f * (partWidth + gap), "RM: ");
}

void UIInventoryBar::draw(sf::RenderWindow &window) {
  window.draw(m_bgRect);
  window.draw(m_borderRect);
  window.draw(m_scrapFill);
  window.draw(m_elecFill);
  window.draw(m_rareFill);
  window.draw(m_scrapText);
  window.draw(m_elecText);
  window.draw(m_rareText);
}
