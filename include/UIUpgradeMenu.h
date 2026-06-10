#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"

class UIUpgradeMenu {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;

    sf::Text m_titleText;
    sf::Text m_scrapsText;
    sf::Text m_upgradesText[4];
    sf::Text m_instructionsText;

    sf::Font m_font;

    void centerTextX(sf::Text& text, float y);

public:
    UIUpgradeMenu();
    UIUpgradeMenu(sf::Vector2f resolution, const sf::Font& font);
    void update(Ship& ship);
    void draw(sf::RenderWindow& window);
};
