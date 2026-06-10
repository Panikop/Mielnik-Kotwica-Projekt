#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"

class UIInventoryMenu {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;

    sf::Text m_titleText;
    sf::Text m_scrapText;
    sf::Text m_electronicsText;
    sf::Text m_rareMetalsText;
    sf::Text m_instructionsText;

    sf::Font m_font;

    void centerTextX(sf::Text& text, float y);

public:
    UIInventoryMenu();
    UIInventoryMenu(sf::Vector2f resolution, const sf::Font& font);
    void update(Ship& ship);
    void draw(sf::RenderWindow& window);
};
