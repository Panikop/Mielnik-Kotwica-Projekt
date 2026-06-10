#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "fraction.h"

class UIFractionsMenu {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;

    sf::Text m_titleText;
    sf::Text m_fractionsText[4];
    sf::Text m_instructionsText;

    sf::Font m_font;

    void centerTextX(sf::Text& text, float y);

public:
    UIFractionsMenu();
    UIFractionsMenu(sf::Vector2f resolution, const sf::Font& font);
    void update(const std::vector<Fraction>& fractions);
    void draw(sf::RenderWindow& window);
};
