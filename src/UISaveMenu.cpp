#include "../include/UISaveMenu.h"
#include <iostream>

UISaveMenu::UISaveMenu() : m_position(0.f, 0.f), m_size(0.f, 0.f) {}

UISaveMenu::UISaveMenu(sf::Vector2f resolution, const sf::Font &font)
    : m_font(font) {
  m_size = sf::Vector2f(400.f, 300.f);
  m_position = sf::Vector2f((resolution.x - m_size.x) / 2.f,
                            (resolution.y - m_size.y) / 2.f);

  m_bgRect.setPosition(m_position);
  m_bgRect.setSize(m_size);
  m_bgRect.setFillColor(sf::Color(10, 15, 20, 235));

  m_borderRect.setPosition(m_position);
  m_borderRect.setSize(m_size);
  m_borderRect.setFillColor(sf::Color::Transparent);
  m_borderRect.setOutlineThickness(2.0f);
  m_borderRect.setOutlineColor(sf::Color(50, 200, 50, 180));

  m_titleText.setFont(m_font);
  m_titleText.setString("ZAPISZ GRE");
  m_titleText.setCharacterSize(26);
  m_titleText.setFillColor(sf::Color(50, 255, 50));
  m_titleText.setStyle(sf::Text::Bold);
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(1.5f);
  centerTextX(m_titleText, m_position.y + 30.f);

  std::string slots[3] = {"SLOT 1", "SLOT 2", "SLOT 3"};
  for (int i = 0; i < 3; ++i) {
    m_slotTexts[i].setFont(m_font);
    m_slotTexts[i].setString(slots[i]);
    m_slotTexts[i].setCharacterSize(20);
    m_slotTexts[i].setStyle(sf::Text::Bold);
    m_slotTexts[i].setOutlineColor(sf::Color::Black);
    m_slotTexts[i].setOutlineThickness(1.f);
    centerTextX(m_slotTexts[i], m_position.y + 100.f + i * 50.f);
  }

  m_instructionsText.setFont(m_font);
  m_instructionsText.setString("Nacisnij [F5] lub [Esc] aby zamknac");
  m_instructionsText.setCharacterSize(14);
  m_instructionsText.setFillColor(sf::Color(150, 160, 175));
  m_instructionsText.setOutlineColor(sf::Color::Black);
  m_instructionsText.setOutlineThickness(1.f);
  centerTextX(m_instructionsText, m_position.y + m_size.y - 30.f);
}

void UISaveMenu::centerTextX(sf::Text &text, float y) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.left + bounds.width / 2.0f,
                 bounds.top + bounds.height / 2.0f);
  text.setPosition(m_position.x + m_size.x / 2.0f, y);
}

int UISaveMenu::update(sf::Vector2f mousePos, bool mouseClicked) {
  int result = -1;
  for (int i = 0; i < 3; ++i) {
    if (m_slotTexts[i].getGlobalBounds().contains(mousePos)) {
      m_slotTexts[i].setFillColor(sf::Color::White);
      if (mouseClicked) {
        result = i + 1; // 1, 2, 3
      }
    } else {
      m_slotTexts[i].setFillColor(sf::Color::Yellow);
    }
  }
  return result;
}

void UISaveMenu::draw(sf::RenderWindow &window) {
  window.draw(m_bgRect);
  window.draw(m_borderRect);
  window.draw(m_titleText);
  for (int i = 0; i < 3; ++i) {
    window.draw(m_slotTexts[i]);
  }
  window.draw(m_instructionsText);
}
