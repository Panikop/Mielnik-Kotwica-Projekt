#include "../include/UIFractionsMenu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

UIFractionsMenu::UIFractionsMenu() : m_position(0.f, 0.f), m_size(0.f, 0.f) {}

UIFractionsMenu::UIFractionsMenu(sf::Vector2f resolution, const sf::Font &font)
    : m_font(font) {
  // Ciemny panel 640x480 na srodku ekranu
  m_size = sf::Vector2f(640.f, 480.f);
  m_position = sf::Vector2f((resolution.x - m_size.x) / 2.f,
                            (resolution.y - m_size.y) / 2.f);

  // Tlo (glassmorphism style)
  m_bgRect.setPosition(m_position);
  m_bgRect.setSize(m_size);
  m_bgRect.setFillColor(sf::Color(10, 15, 20, 235));

  // Ramka w odcieniach zlota/zolci
  m_borderRect.setPosition(m_position);
  m_borderRect.setSize(m_size);
  m_borderRect.setFillColor(sf::Color::Transparent);
  m_borderRect.setOutlineThickness(2.0f);
  m_borderRect.setOutlineColor(sf::Color(218, 165, 32, 180));

  // Tytul
  m_titleText.setFont(m_font);
  m_titleText.setString("STOSUNKI Z FRAKCJAMI");
  m_titleText.setCharacterSize(26);
  m_titleText.setFillColor(sf::Color(218, 165, 32));
  m_titleText.setStyle(sf::Text::Bold);
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(1.5f);
  centerTextX(m_titleText, m_position.y + 40.f);

  // Inicjalizacja tekstow dla 4 frakcji
  for (int i = 0; i < 4; ++i) {
    m_fractionsText[i].setFont(m_font);
    m_fractionsText[i].setCharacterSize(18);
    m_fractionsText[i].setStyle(sf::Text::Bold);
    m_fractionsText[i].setOutlineColor(sf::Color::Black);
    m_fractionsText[i].setOutlineThickness(1.f);
  }

  // Instrukcja sterowania
  m_instructionsText.setFont(m_font);
  m_instructionsText.setString("Nacisnij [F] lub [Esc] aby zamknac panel");
  m_instructionsText.setCharacterSize(14);
  m_instructionsText.setFillColor(sf::Color(150, 160, 175));
  m_instructionsText.setOutlineColor(sf::Color::Black);
  m_instructionsText.setOutlineThickness(1.f);
  centerTextX(m_instructionsText, m_position.y + m_size.y - 40.f);
}

void UIFractionsMenu::centerTextX(sf::Text &text, float y) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.left + bounds.width / 2.0f,
                 bounds.top + bounds.height / 2.0f);
  text.setPosition(m_position.x + m_size.x / 2.0f, y);
}

void UIFractionsMenu::update(const std::vector<Fraction> &fractions) {
  for (size_t i = 0; i < 4 && i < fractions.size(); ++i) {
    int trust = fractions[i].getTrustLevel();
    std::string fractionName = fractions[i].getName();

    std::stringstream ss;
    ss << fractionName << "  |  Zaufanie: " << trust << " %";
    m_fractionsText[i].setString(ss.str());

    // Zmiana koloru w zaleznosci od poziomu zaufania
    if (trust >= 70) {
      m_fractionsText[i].setFillColor(
          sf::Color(100, 255, 100)); // Zielony - sojuszniczy
    } else if (trust >= 30) {
      m_fractionsText[i].setFillColor(
          sf::Color(230, 230, 230)); // Bialy - neutralny
    } else {
      m_fractionsText[i].setFillColor(
          sf::Color(255, 100, 100)); // Czerwony - wrogi
    }

    centerTextX(m_fractionsText[i], m_position.y + 120.f + i * 70.f);
  }
}

void UIFractionsMenu::draw(sf::RenderWindow &window) {
  window.draw(m_bgRect);
  window.draw(m_borderRect);
  window.draw(m_titleText);

  for (int i = 0; i < 4; ++i) {
    window.draw(m_fractionsText[i]);
  }

  window.draw(m_instructionsText);
}
