#include "mainmenu.h"
#include <iostream>

MainMenu::MainMenu(sf::Vector2f resolution, sf::Font &font)
    : m_resolution(resolution), m_font(font) {
  m_titleText.setFont(m_font);
  m_titleText.setString("PANIC ANCHOR");
  m_titleText.setCharacterSize(80);
  m_titleText.setFillColor(sf::Color::Red);
  m_titleText.setStyle(sf::Text::Bold);
  sf::FloatRect titleBounds = m_titleText.getLocalBounds();
  m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f,
                        titleBounds.top + titleBounds.height / 2.f);
  m_titleText.setPosition(resolution.x / 2.f, resolution.y * 0.2f);

  auto initText = [&](sf::Text& t, const std::string& str, float yPos) {
      t.setFont(m_font);
      t.setString(str);
      t.setCharacterSize(35);
      t.setFillColor(sf::Color::Yellow);
      sf::FloatRect bounds = t.getLocalBounds();
      t.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
      t.setPosition(resolution.x / 2.f, yPos);
  };

  initText(m_newGameText, "Nowa Gra", resolution.y * 0.45f);
  initText(m_loadSlot1, "Wczytaj Slot 1", resolution.y * 0.55f);
  initText(m_loadSlot2, "Wczytaj Slot 2", resolution.y * 0.65f);
  initText(m_loadSlot3, "Wczytaj Slot 3", resolution.y * 0.75f);

  if (!m_backgroundTexture.loadFromFile("textures/menu_bg.png")) {
    std::cerr << "Blad ladowania textures/menu_bg.png" << std::endl;
  }
  m_backgroundShape.setSize(resolution);
  m_backgroundShape.setTexture(&m_backgroundTexture);
}

int MainMenu::update(sf::Vector2f mousePos, bool mouseClicked,
                      state &activeState) {
  int result = -1;

  auto handleText = [&](sf::Text& t, int retVal) {
      if (t.getGlobalBounds().contains(mousePos)) {
          t.setFillColor(sf::Color::White);
          if (mouseClicked) {
              activeState = state::STATEK;
              result = retVal;
          }
      } else {
          t.setFillColor(sf::Color::Yellow);
      }
  };

  handleText(m_newGameText, 0);
  handleText(m_loadSlot1, 1);
  handleText(m_loadSlot2, 2);
  handleText(m_loadSlot3, 3);

  return result;
}

void MainMenu::draw(sf::RenderWindow &window) {
  window.draw(m_backgroundShape);
  window.draw(m_titleText);
  window.draw(m_newGameText);
  window.draw(m_loadSlot1);
  window.draw(m_loadSlot2);
  window.draw(m_loadSlot3);
}
