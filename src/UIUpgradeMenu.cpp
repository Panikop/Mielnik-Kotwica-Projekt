#include "UIUpgradeMenu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

UIUpgradeMenu::UIUpgradeMenu() : m_position(0.f, 0.f), m_size(0.f, 0.f) {}

UIUpgradeMenu::UIUpgradeMenu(sf::Vector2f resolution, const sf::Font &font)
    : m_font(font) {
  // Ustawienie wymiarów i pozycji (wyśrodkowany panel 640x480)
  m_size = sf::Vector2f(640.f, 480.f);
  m_position = sf::Vector2f((resolution.x - m_size.x) / 2.f,
                            (resolution.y - m_size.y) / 2.f);

  // Panel tła (ciemny glassmorphism)
  m_bgRect.setPosition(m_position);
  m_bgRect.setSize(m_size);
  m_bgRect.setFillColor(sf::Color(10, 10, 15, 230));

  // Obramowanie z neonowym błękitem
  m_borderRect.setPosition(m_position);
  m_borderRect.setSize(m_size);
  m_borderRect.setFillColor(sf::Color::Transparent);
  m_borderRect.setOutlineThickness(2.0f);
  m_borderRect.setOutlineColor(sf::Color(0, 191, 255, 180));

  // Tytuł menu
  m_titleText.setFont(m_font);
  m_titleText.setString("ULEPSZENIA STATKU");
  m_titleText.setCharacterSize(24);
  m_titleText.setFillColor(sf::Color(0, 191, 255));
  m_titleText.setStyle(sf::Text::Bold);
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(1.5f);
  centerTextX(m_titleText, m_position.y + 30.f);

  // Licznik złomu
  m_scrapsText.setFont(m_font);
  m_scrapsText.setCharacterSize(16);
  m_scrapsText.setFillColor(sf::Color(218, 165, 32));
  m_scrapsText.setStyle(sf::Text::Bold);
  m_scrapsText.setOutlineColor(sf::Color::Black);
  m_scrapsText.setOutlineThickness(1.f);

  // Opcje ulepszeń
  for (int i = 0; i < 4; ++i) {
    m_upgradesText[i].setFont(m_font);
    m_upgradesText[i].setCharacterSize(15);
    m_upgradesText[i].setStyle(sf::Text::Bold);
    m_upgradesText[i].setOutlineColor(sf::Color::Black);
    m_upgradesText[i].setOutlineThickness(1.f);
  }

  // Instrukcja sterowania
  m_instructionsText.setFont(m_font);
  m_instructionsText.setString("Nacisnij klawisz [1-4] aby kupic ulepszenie  | "
                               " Nacisnij [U] lub [Esc] aby zamknac");
  m_instructionsText.setCharacterSize(13);
  m_instructionsText.setFillColor(sf::Color(150, 160, 175));
  m_instructionsText.setOutlineColor(sf::Color::Black);
  m_instructionsText.setOutlineThickness(1.f);
  centerTextX(m_instructionsText, m_position.y + m_size.y - 35.f);
}

void UIUpgradeMenu::centerTextX(sf::Text &text, float y) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.left + bounds.width / 2.0f,
                 bounds.top + bounds.height / 2.0f);
  text.setPosition(m_position.x + m_size.x / 2.0f, y);
}

void UIUpgradeMenu::update(Ship &ship) {
  int currentScraps = static_cast<int>(ship.getCurrentScrap());
  int maxStorage = static_cast<int>(ship.getMaxScrap());

  std::stringstream ss;
  ss << "Posiadany zlom w cargo: " << currentScraps << " / " << maxStorage
     << " t";
  m_scrapsText.setString(ss.str());
  centerTextX(m_scrapsText, m_position.y + 80.f);

  int lvlHP = ship.getHealthUpgradeLevel();
  int lvlNitro = ship.getNitroUpgradeLevel();
  int lvlStorage = ship.getStorageUpgradeLevel();
  int lvlSpeed = ship.getSpeedUpgradeLevel();

  sf::Vector3f costHP = ship.getHealthUpgradeCost();
  sf::Vector3f costNitro = ship.getNitroUpgradeCost();
  sf::Vector3f costStorage = ship.getStorageUpgradeCost();
  sf::Vector3f costSpeed = ship.getSpeedUpgradeCost();

  float maxHP = ship.getMaxHealth();
  float maxNitro = ship.getMaxNitro();
  float maxCargo = ship.getMaxScrap();
  float speed = ship.getShipSpeed();

  std::string names[4] = {"[1] Maks. Zdrowie (HP)", "[2] Maks. Nitro (Boost)",
                          "[3] Pojemnosc Cargo", "[4] Predkosc Silnika"};

  std::stringstream valSS[4];
  valSS[0] << maxHP << " -> " << (maxHP + 25) << "  (Poziom " << lvlHP << ")";
  valSS[1] << maxNitro << " -> " << (maxNitro + 15) << "  (Poziom " << lvlNitro
           << ")";
  valSS[2] << maxCargo << " -> " << (maxCargo + 50) << "  (Poziom "
           << lvlStorage << ")";
  valSS[3] << speed << " -> " << (speed + 30) << "  (Poziom " << lvlSpeed
           << ")";

  sf::Vector3f costs[4] = {costHP, costNitro, costStorage, costSpeed};

  int curElec = static_cast<int>(ship.getCurrentElectronics());
  int curRare = static_cast<int>(ship.getCurrentRareMetals());

  for (int i = 0; i < 4; ++i) {
    std::stringstream rowSS;
    rowSS << names[i] << ":  " << valSS[i].str()
          << "\nKoszt: " << static_cast<int>(costs[i].x) << " Zlom | "
          << static_cast<int>(costs[i].y) << " Elektr | "
          << static_cast<int>(costs[i].z) << " Rzad. Met.";
    m_upgradesText[i].setString(rowSS.str());

    if (currentScraps >= costs[i].x && curElec >= costs[i].y &&
        curRare >= costs[i].z) {
      m_upgradesText[i].setFillColor(
          sf::Color(100, 255, 100)); // Zielony - dostepny
    } else {
      m_upgradesText[i].setFillColor(
          sf::Color(255, 100, 100)); // Czerwony - niedostepny
    }

    centerTextX(m_upgradesText[i], m_position.y + 125.f + i * 75.f);
  }
}

void UIUpgradeMenu::draw(sf::RenderWindow &window) {
  window.draw(m_bgRect);
  window.draw(m_borderRect);
  window.draw(m_titleText);
  window.draw(m_scrapsText);

  for (int i = 0; i < 4; ++i) {
    window.draw(m_upgradesText[i]);
  }

  window.draw(m_instructionsText);
}
