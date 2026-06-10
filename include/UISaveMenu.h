#pragma once
#include <SFML/Graphics.hpp>

class UISaveMenu {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;

    sf::Text m_titleText;
    sf::Text m_slotTexts[3];
    sf::Text m_instructionsText;

    sf::Font m_font;

    void centerTextX(sf::Text& text, float y);

public:
    UISaveMenu();
    UISaveMenu(sf::Vector2f resolution, const sf::Font& font);

    int update(sf::Vector2f mousePos, bool mouseClicked);
    void draw(sf::RenderWindow& window);
};
