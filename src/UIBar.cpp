#include "../include/UIBar.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>

UIBar::UIBar()
    : m_position(0.f, 0.f), m_size(0.f, 0.f), m_currentValue(0.f),
      m_lagValue(0.f), m_maxValue(1.f), m_isCritical(false),
      m_isOnCooldown(false), m_pulseTime(0.f) {}

UIBar::UIBar(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor,
             const std::string &label, const sf::Font &font)
    : m_position(position), m_size(size), m_baseColor(fillColor),
      m_currentValue(0.f), m_lagValue(0.f), m_maxValue(100.f),
      m_isCritical(false), m_isOnCooldown(false), m_pulseTime(0.f) {
  // Ustawienie tła (ciemna ramka w stylu glassmorphism)
  m_bgRect.setPosition(m_position);
  m_bgRect.setSize(m_size);
  m_bgRect.setFillColor(
      sf::Color(15, 15, 22, 200)); // półprzezroczysty ciemnoszary/niebieski

  // Ustawienie obramowania/ramki
  m_borderRect.setPosition(m_position);
  m_borderRect.setSize(m_size);
  m_borderRect.setFillColor(sf::Color::Transparent);
  m_borderRect.setOutlineThickness(1.5f);
  m_borderRect.setOutlineColor(
      sf::Color(70, 80, 95, 200)); // metaliczny chłodny szary

  // Kolor opóźnienia (lag) to półprzezroczysta wersja koloru wypełnienia
  m_lagColor = sf::Color(fillColor.r, fillColor.g, fillColor.b, 100);

  // Margines wewnątrz paska (zostawia 3px wolnej przestrzeni wokół wypełnienia)
  float padding = 3.f;
  sf::Vector2f fillSize(m_size.x - 2.f * padding, m_size.y - 2.f * padding);
  sf::Vector2f fillPos = m_position + sf::Vector2f(padding, padding);

  // Ustawienie prostokąta paska opóźnienia
  m_lagRect.setPosition(fillPos);
  m_lagRect.setSize(sf::Vector2f(0.f, fillSize.y));
  m_lagRect.setFillColor(m_lagColor);

  // Ustawienie prostokąta właściwego paska
  m_fillRect.setPosition(fillPos);
  m_fillRect.setSize(sf::Vector2f(0.f, fillSize.y));
  m_fillRect.setFillColor(m_baseColor);

  // Ustawienie odblasku (błyszczący efekt szkła na górnej połowie paska)
  m_sheenRect.setPosition(fillPos);
  m_sheenRect.setSize(sf::Vector2f(0.f, fillSize.y / 2.f));
  m_sheenRect.setFillColor(
      sf::Color(255, 255, 255, 30)); // bardzo jasny biały odcień

  // Ustawienie elementów tekstowych
  int charSize = static_cast<int>(m_size.y * 0.45f);
  if (charSize < 10)
    charSize = 10;
  if (charSize > 14)
    charSize = 14;

  m_labelText.setFont(font);
  m_labelText.setString(label);
  m_labelText.setCharacterSize(charSize);
  m_labelText.setFillColor(
      sf::Color(230, 240, 255, 255)); // elegancka, jasna złamana biel
  m_labelText.setStyle(sf::Text::Bold);
  m_labelText.setOutlineColor(sf::Color::Black);
  m_labelText.setOutlineThickness(1.f);
  // Wyrównanie etykiety pionowo do środka po lewej stronie
  sf::FloatRect labelBounds = m_labelText.getLocalBounds();
  m_labelText.setOrigin(0.f, labelBounds.top + labelBounds.height / 2.f);
  m_labelText.setPosition(m_position.x + 8.f, m_position.y + m_size.y / 2.f);

  m_valueText.setFont(font);
  m_valueText.setCharacterSize(charSize - 1);
  m_valueText.setFillColor(sf::Color(230, 240, 255, 220));
  m_valueText.setStyle(sf::Text::Bold);
  m_valueText.setOutlineColor(sf::Color::Black);
  m_valueText.setOutlineThickness(1.f);
}

void UIBar::update(float dt, float currentValue, float maxValue,
                   bool isOnCooldown) {
  m_currentValue = currentValue;
  m_maxValue = (maxValue > 0.f) ? maxValue : 1.f;
  m_isOnCooldown = isOnCooldown;

  // Ograniczenie bieżącej wartości do przedziału [0, max]
  if (m_currentValue < 0.f)
    m_currentValue = 0.f;
  if (m_currentValue > m_maxValue)
    m_currentValue = m_maxValue;

  // Inicjalizacja wartości opóźnienia w pierwszej klatce lub przy utracie
  // synchronizacji
  if (m_lagValue < 0.01f && m_currentValue > 0.01f) {
    m_lagValue = m_currentValue;
  }

  // Powolna interpolacja wartości opóźnienia w dół.
  // Jeśli bieżąca wartość jest większa niż opóźnienie (np. leczenie lub
  // ładowanie), opóźnienie dogania ją natychmiast
  if (m_currentValue >= m_lagValue) {
    m_lagValue = m_currentValue;
  } else {
    // Powolne przejście opóźnienia
    m_lagValue -= (m_lagValue - m_currentValue) * dt * 2.0f;
    if (m_lagValue < m_currentValue)
      m_lagValue = m_currentValue;
  }

  // Sprawdzenie krytycznego poziomu zdrowia (poniżej 25%)
  m_isCritical = (m_labelText.getString() == "HP" &&
                  (m_currentValue / m_maxValue) < 0.25f);

  m_pulseTime += dt;

  // Aktualizacja koloru ramki w zależności od stanów
  if (m_isCritical) {
    // Czerwona migająca ramka przy niskim poziomie zdrowia
    float pulse = (std::sin(m_pulseTime * 7.f) + 1.f) / 2.f;
    m_borderRect.setOutlineColor(
        sf::Color(255, 40, 40, static_cast<sf::Uint8>(100 + 155 * pulse)));
  } else if (m_isOnCooldown) {
    // Niebieska migająca ramka podczas czasu odnowienia (cooldown)
    float pulse = (std::sin(m_pulseTime * 5.f) + 1.f) / 2.f;
    m_borderRect.setOutlineColor(
        sf::Color(40, 100, 255, static_cast<sf::Uint8>(100 + 155 * pulse)));
  } else {
    // Domyślna metaliczna ramka
    m_borderRect.setOutlineColor(sf::Color(70, 80, 95, 200));
  }

  // Dynamiczne dostosowanie kolorów
  if (m_isOnCooldown) {
    m_fillRect.setFillColor(sf::Color(30, 80, 220)); // Ciemnoniebieski
  } else {
    m_fillRect.setFillColor(m_baseColor);
  }

  // Obliczanie szerokości
  float padding = 3.f;
  float maxFillWidth = m_size.x - 2.f * padding;
  float fillWidth = maxFillWidth * (m_currentValue / m_maxValue);
  float lagWidth = maxFillWidth * (m_lagValue / m_maxValue);

  m_fillRect.setSize(sf::Vector2f(fillWidth, m_size.y - 2.f * padding));
  m_lagRect.setSize(sf::Vector2f(lagWidth, m_size.y - 2.f * padding));
  m_sheenRect.setSize(
      sf::Vector2f(fillWidth, (m_size.y - 2.f * padding) / 2.f));

  // Aktualizacja tekstu wartości
  std::stringstream ss;
  if (m_labelText.getString() == "NITRO") {
    // Wyświetlanie jako procent
    ss << std::fixed << std::setprecision(0)
       << (m_currentValue / m_maxValue * 100.f) << "%";
  } else {
    ss << std::fixed << std::setprecision(0) << m_currentValue << " / "
       << m_maxValue;
  }
  m_valueText.setString(ss.str());

  // Wyrównanie tekstu wartości do prawej strony
  sf::FloatRect valBounds = m_valueText.getLocalBounds();
  m_valueText.setOrigin(valBounds.width,
                        valBounds.top + valBounds.height / 2.f);
  m_valueText.setPosition(m_position.x + m_size.x - 8.f,
                          m_position.y + m_size.y / 2.f);
}

void UIBar::draw(sf::RenderWindow &window) {
  // Rysowanie tła i obramowania
  window.draw(m_bgRect);
  window.draw(m_borderRect);

  // Rysowanie paska opóźnienia (jeśli jest większy niż bieżący pasek)
  if (m_lagRect.getSize().x > m_fillRect.getSize().x) {
    window.draw(m_lagRect);
  }

  // Rysowanie właściwego paska
  if (m_fillRect.getSize().x > 0.f) {
    window.draw(m_fillRect);
    window.draw(m_sheenRect); // rysowanie błyszczącego efektu szkła na wierzchu
  }

  // Rysowanie tekstu
  window.draw(m_labelText);
  window.draw(m_valueText);
}
